from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir
from datetime import datetime
import time

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():


    # query database for username
    #rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

    #SESSION["user_id"] = rows[1]["id"]
    username = db.execute("SELECT username FROM users WHERE id = :id", id=session["user_id"])
    # obtain cash info from users database
    cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
    grandtotal = cash[0]["cash"]


    # obtain stock info from portfolio database
    stocks = db.execute("SELECT symbol, shares FROM portfolio WHERE id=:id", id=session["user_id"])
    #check = db.execute("SELECT username from portfolio where id=:id", id=session["user_id"])

    # for every stock in the user's portfolio, assign dict key/values for use in html/jinja
    for stock in stocks:
        symbol = str(stock["symbol"])
        shares = int(stock["shares"])
        name = ""
        price = ""
        total = ""
        quote = lookup(symbol)
        stock["name"] = quote["name"]
        stock["price"] = "{:.2f}".format(quote["price"])
        stock["total"] = "{:.2f}".format(quote["price"] * shares)
        stock["grandtotal"] = (lookup(symbol)["price"] * shares)
        grandtotal += stock["grandtotal"]

    # format grandtotal to force 2 decimal places
    grandtotal = "{:.2f}".format(grandtotal)

    # render index page with some given values
    return render_template("index.html", stocks = stocks, cash = cash[0]["cash"], grandtotal = grandtotal)



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():


    """Buy shares of stock."""
    if request.method == "POST":

        symbol=request.form.get("Symbol")
        shares=request.form.get("Shares")

        """ensure symbol was inputed"""
        if not request.form.get("Symbol") or lookup(request.form.get("Symbol")) == None:
            return render_template("apology.html", name="must provide symbol/symbol does not exist")
        elif not request.form.get("Shares") or int(shares) < 0:
            return render_template("apology.html", name="shares must be a positive integer")

        #Sym = lookup(symbol)["name"]
        #Price = lookup(symbol)["price"]
        Sh = int(shares)

        # query database for cash
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        username = db.execute("SELECT username FROM users WHERE id = :id", id=session["user_id"])[0]["username"]
        Total = float(lookup(symbol)["price"]) * int(shares)
        if float(cash[0]["cash"]) >= Total:

            #update the users portfolio
            #db.execute("UPDATE portfolio SET shares = shares + :shares WHERE id = :id AND symbol = :symbol", \
            #id=1,symbol=symbol,shares=shares)
            db.execute("UPDATE users SET cash = cash - :Total WHERE id = :id", id=session["user_id"], Total = Total)

            check = db.execute("SELECT shares from portfolio WHERE id=:id AND symbol=:symbol",id=session["user_id"], symbol = symbol)

            if not check:
                result = db.execute("INSERT INTO portfolio (username, symbol, name, shares, price, total, transacted, id)  \
                VALUES(:username, :symbol, :name, :shares, :price, :total, datetime('now'), :id)",\
                username = username, symbol=symbol, name=symbol, shares=int(shares), price=str(lookup(symbol)["price"]), \
                total = (Total), id=session["user_id"])
            elif check:



                shares_total = check[0]["shares"] + int(shares)
                result = db.execute("UPDATE portfolio SET shares =:shares WHERE id=:id AND symbol=:symbol", \
                id=session["user_id"], shares = shares_total,symbol = symbol)

            #update history
            result = db.execute("INSERT INTO history (username, symbol,shares, price, transacted, id)  \
            VALUES(:username, :symbol, :shares, :price, datetime('now'), :id)",\
            username = username, symbol=symbol, shares=int(shares), price=str(lookup(symbol)["price"]), \
            id=session["user_id"])
        else:
            return render_template("apology.html", name="Not enough cash")
        flash('Bought!')
        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    elif request.method == "GET":
        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    histories = db.execute("SELECT * FROM history WHERE id=:id", id=session["user_id"])

    return render_template("history.html", histories=histories)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        result = request.form.get("Symbol")
        if not result:
            return apology("Enter symbol")

        name= lookup(request.form.get("Symbol"))["name"]
        price= lookup(request.form.get("Symbol"))["price"]

        if not name or not price:
            return apology("Invalid Symbol")

        return render_template("quoted.html", symbol=name, price=usd(price))

    elif request.method == "GET":
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        """Register user."""
        # forget any user_id
        session.clear()

        # ensure username was submitted
        if not request.form.get("username"):
            return render_template("apology.html", name="you must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return render_template("apology.html", name="you must provide password")

        # ensure password was submitted
        elif not request.form.get("passwordCheck"):
            return render_template("apology.html", name="you must enter password (again)")

        # ensure passwords match
        elif request.form.get("password") != request.form.get("passwordCheck"):
            return render_template("apology.html", name="the Passwords don't match")

        hash = pwd_context.hash(request.form.get("password"))


        # add username to the database
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)",
        username=request.form.get("username"), hash=hash)


        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        if not result:
            return render_template("apology.html", name="Username Taken!")

        # remember which user has logged in
        session["user_id"] = result

        flash('Registered!')
        # redirect user to home page
        return redirect(url_for("index"))


    elif request.method == "GET":
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    """Sell shares of stock."""
    if request.method == "POST":

        symbol=request.form.get("Symbol")
        shares=request.form.get("Shares")
        username = db.execute("SELECT username FROM users WHERE id = :id", id=session["user_id"])[0]["username"]

        """ensure symbol was inputed"""
        if not request.form.get("Symbol") or lookup(request.form.get("Symbol")) == None:
            return render_template("apology.html", name="must provide symbol/symbol does not exist")
        elif not request.form.get("Shares") or int(shares) < 0:
            return render_template("apology.html", name="shares must be a positive integer")

        # query database for cash
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
        symbol_q = db.execute("SELECT shares FROM portfolio WHERE id = :id AND symbol = :symbol", id=session["user_id"], symbol = symbol)

        Total = float(lookup(symbol)["price"]) * int(shares)
        Sh = int(shares)

        if not symbol_q or symbol_q[0]["shares"] < int(shares):
            return apology("sorry, you don't have this stock or not enough shares")

        #update history
        result = db.execute("INSERT INTO history (username, symbol,shares, price, transacted, id)  \
        VALUES(:username, :symbol, :shares, :price, datetime('now'), :id)",\
        username = username, symbol=symbol, shares=-Sh, price=str(lookup(symbol)["price"]), \
        id=session["user_id"])

        db.execute("UPDATE users SET cash = cash + :Total WHERE id = :id", id=session["user_id"], Total = Total)
        share_total = symbol_q[0]["shares"] - int(shares)
        if share_total == 0:
            db.execute("DELETE FROM portfolio \
                        WHERE id=:id AND symbol=:symbol", \
                        id=session["user_id"], \
                        symbol=symbol)
        else:
            db.execute("UPDATE portfolio SET shares = shares - :Sh WHERE id = :id", id=session["user_id"], Sh =Sh)
            username = db.execute("SELECT username FROM users WHERE id = :id", id=session["user_id"])



        flash('Sold!')
        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    elif request.method == "GET":
        return render_template("sell.html")

