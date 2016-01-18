#include <stdio.h>
#include <string.h>
#include <cs50.h>

int main(int argc, string argv[])
{
    if (argc != 2) 
    {
        return 1;
    }

    string text = argv[1];
    char encrypted[strlen(text)];

    int max = (int) 'z' - (int) 'a';
    int offset = 15;

    for (int i = 0, l = strlen(text); i < l; i++)
    {
        int letter = (int) text[i];
        if (letter - max < 0)

        printf("Encrypting letter %i\n", (int) text[i]);
//        printf("Offset: %i\n", (100 % 26));
        //int offset = 10;
        encrypted[i] = (char) ((text[i] + 100) % 26);
        printf("Result: %c\n", encrypted[i]);
    }
    
    printf("Input: %s\n", text);
    printf("Output: %s\n", encrypted);
}
