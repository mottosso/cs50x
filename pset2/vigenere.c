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

    string key = argv[1];
    int keyLength = strlen(key);

    for (int i = 0; i < keyLength; i++)
    {
        if (!isalpha(key[i]))
        {
            printf("Argument must contain only alphabetical characters\n");
            return 1;
        }
    }

    string source = GetString();
    int sourceLength = strlen(source);

    int sourceRoot, keyRoot, sourceDelta, keyDelta, encrypted;
    sourceRoot = keyRoot = sourceDelta = keyDelta = encrypted = 0;

    for (int i = 0, j = 0; i < sourceLength; i++)
    {
        if (!isalpha(source[i]))
        {
            printf("%c", source[i]);
            continue;
        }

        // Uppercase letters are offset differently from uppercase
        keyRoot = isupper(key[j]) ? 65 : 97;
        sourceRoot = isupper(source[i]) ? 65 : 97;

        keyDelta = key[j] - keyRoot;
        sourceDelta = source[i] - sourceRoot;

        encrypted = (sourceDelta + keyDelta) % 26;

        // Re-apply delta
        encrypted += sourceRoot;

        printf("%c", encrypted);

        j = (j + 1) % keyLength;
    }

    printf("\n");

    return 0;
}
