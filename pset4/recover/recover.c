#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define IS_JPEG(buffer)  buffer[0] == 0xff && buffer[1] == 0xd8 && \
                                   buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0

typedef uint8_t  BYTE;
int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile \n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];
    FILE *img = NULL;

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    BYTE buffer[11200];
    int count = 0;



    //fread(buffer, 512, 1, inptr);
    for(int i = 0; fread(buffer, 512, 2, inptr) != 1; i++)
    {
        fread(buffer, 512, 1, inptr);
    next:    if(IS_JPEG(buffer))
        {

            char filename[8];
            sprintf(filename, "%03i.jpg", count);
            count++;
            img = fopen(filename, "w");

            do
            {
                fwrite(buffer, 512, 1, img);
                fread(buffer, 512, 1, inptr);

            }
            while(!(IS_JPEG(buffer)) && !feof(inptr) && img != NULL);
            fread(buffer, 512, 1, img);
            // close current filename
            fclose(img);
            goto next;
        }
        if(feof(inptr))
            return 0;             //fclose(img);
    }

    // close infile
    fclose(inptr);

}