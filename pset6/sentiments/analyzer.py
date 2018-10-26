import nltk
import os
import sys


class Analyzer():
    """Implements sentiment analysis."""


    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        # TODO
        self.positives = []
        self.negatives = []

        #load positive words
        with open(positives, 'r') as lines:

            for line in lines:
                if not line.startswith(';'):

                    word = line.strip()
                    self.positives.append(word)

        #load negative words
        with open(negatives) as lines:

            for line in lines:
                if not line.startswith(';'):

                    word = line.strip()
                    self.negatives.append(word)



    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        # TODO

        word = text.lower()
        score, positive, negative, neutral = 0, 0, 0, 0

        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(word)

        for token in tokens:
            if token in w.positives:
                positive +=1
            elif token in w.negatives:
                negative +=1
            else:
                neutral += 1

        score = positive - negative

        return score


# absolute paths to lists
positives = os.path.join(sys.path[0], "positive-words.txt")
negatives = os.path.join(sys.path[0], "negative-words.txt")
w = Analyzer(positives, negatives)

