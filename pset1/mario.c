#include <stdio.h>
#include "cs50.h"

int main(void)
{    
    int height = 0;

    do
    {
        printf("height: ");
        height = GetInt();
    } while (height < 0 || height > 23);
    
    if (height == 0)
    {
        return 0;
    }

    int spaces, hashes = 0;

    for (int i = 0; i < height; i++)
    {
        spaces = height - i;
        hashes = height - spaces;

        for (int i = 1; i < spaces; i++)
        {
            // Spaces start at 1 to eliminate the first column
            // ..Can't wrap head around why it works this way..
            printf(" ");
        }

        for (int i = 0; i < hashes + 2; i++)
        {
            printf("#");
        }

        printf("\n");
    }

    return 0;
}
