#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cs50.h"

int main(int argc, string argv[])
{
    string name = GetString();

    printf("%c", toupper(name[0]));
    for (int i = 1, l = strlen(name); i < l; i++)
    {
        if (name[i] == ' ')
        {
            printf("%c", toupper(name[i + 1]));
        }
    }
    printf("\n");
}
