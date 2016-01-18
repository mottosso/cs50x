#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "cs50.h"

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Must pass a single argument.\n");
        return 1;
    }

    int k = atoi(argv[1]);
    string p = GetString();
    
    int root = 0, delta = 0, c = 0;
    for (int i = 0, n = strlen(p); i < n; i++)
    {
        if (!isalpha(p[i]))
        {
            printf("%c", p[i]);
            continue;
        }
    
        // Uppercase letters are offset differently from uppercase
        root = isupper(p[i]) ? 65 : 97;

        delta = p[i] - root;
        c = root + ((delta + k) % 26);

        printf("%c", c);
    }

    printf("\n");

    return 0;
}
