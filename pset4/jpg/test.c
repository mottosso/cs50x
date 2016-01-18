#include <stdio.h>


int main(void)
{
    char buffer[8];

    for (int i = 1; i <= 10; i++)
    {
        sprintf(buffer, "%03d.jpg", i);
        printf("%s\n", buffer);
    }
    return 0;
}