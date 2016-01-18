/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        printf("Usage: no command-line arguments required.");
        return 1;
    }

    FILE* inptr = fopen("card.raw", "r");
    FILE* outptr = NULL;

    if (inptr == NULL)
    {
        printf("Could not open card.raw for reading.\n");
        return 1;
    }

    uint16_t signatureA = 0xff + 0xd8 + 0xff + 0xe0;
    uint16_t signatureB = 0xff + 0xd8 + 0xff + 0xe1;

    // delay writing until first valid signature
    bool foundFirstSignature = false;
    
    // keep track of which file we're writing to
    int fnumber = 0;

    while (true)
    {
        // read 512 bytes into the free store, as opposed
        // to the stack due to it's considerable size.
        uint8_t* buffer = (uint8_t*) malloc(512);

        if (fread(buffer, 512, 1, inptr) != 1)
        {
            // not enough blocks to justify continuation of jpeg
            fclose(outptr);
            free(buffer);
            break;
        }

        // determine signature
        uint16_t signature = buffer[0] + buffer[1] + buffer[2] + buffer[3];

        // is this the start of a new file?
        if (signature == signatureA || signature == signatureB)
        {
            // indicate that the first jpeg has now been found
            // and we can now start outputting files.
            foundFirstSignature = true;

            // close previous file
            if (outptr != NULL)
            {
                fclose(outptr);
            }

            // open next file
            char fname[8];
            sprintf(fname, "%03d.jpg", fnumber);
            outptr = fopen(fname, "w");

            // error handling
            if (outptr == NULL)
            {
                printf("Could not open %s for writing.\n", fname);
                fclose(inptr);
                free(buffer);
                return 1;
            }

            fnumber += 1;
        }

        if (foundFirstSignature)
        {
            // write current block to current file
            fwrite(buffer, 512, 1, outptr);
        }

        free(buffer);
    }

    fclose(inptr);

    return 0;
}
