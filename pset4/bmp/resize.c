/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Resizes a BMP.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];

    if (n < 1 || n > 100)
    {
        printf("Usage: n must be between 1-100\n");
        return 1;
    }

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER inBf, outBf;
    fread(&inBf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER inBi, outBi;
    fread(&inBi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (inBf.bfType != 0x4d42 || inBf.bfOffBits != 54 || inBi.biSize != 40 || 
        inBi.biBitCount != 24 || inBi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // create new output headers
    outBf = inBf;
    outBi = inBi;

    outBi.biWidth *= n;
    outBi.biHeight *= n;

    int inPadding =  (4 - (inBi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outPadding =  (4 - (outBi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    outBi.biSizeImage = (outBi.biWidth * sizeof(RGBTRIPLE) + outPadding)
                        * abs(outBi.biHeight);
    outBf.bfSize = outBi.biSizeImage
                   + sizeof(BITMAPFILEHEADER)
                   + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&outBf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&outBi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(inBi.biHeight); i < biHeight; i++)
    {
        // repeat row n-times
        for (int j = 0; j < n; j++)
        {
            // iterate over pixels in scanline
            for (int k = 0; k < inBi.biWidth; k++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // repeat column n-times
                for (int l = 0; l < n; l++)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                } 
            }

            for (int n = 0; n < outPadding; n++)
            {
                fputc(0x00, outptr);
            }

            if (j < n - 1)
            {
                fseek(inptr, -inBi.biWidth * sizeof(RGBTRIPLE), SEEK_CUR);
            }
        }

        fseek(inptr, inPadding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
