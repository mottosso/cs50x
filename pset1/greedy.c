#include <stdio.h>
#include <limits.h>
#include <math.h>
#include "cs50.h"


int main(void)
{
    float dollars = 0;
    
    while (true)
    {
        printf("How much change is owed? (0 to cancel)\n");
        dollars = GetFloat();

        if (dollars == 0)
        {
            return 0;
        } else if (dollars <= 0)
        {
            printf("It can't be a negative number.\n");
            continue;
        } else if (dollars * 100 > INT_MAX)
        {
            printf("I can only count to %i\n", INT_MAX);
            continue;
        }

        break;
    }
   
    // Use round() to maintain precision when truncating.
    int cents = round(dollars * 100.0);
    int quarters = 0, dimes = 0, nickles = 0, pennies = 0;
    while (cents > 0)
    {
        if (cents >= 25)
        {
            quarters += 1;
            cents -= 25;
        } else if (cents >= 10)
        {
            dimes += 1;
            cents -= 10;
        } else if (cents >= 5)
        {
            nickles += 1;
            cents -= 5;
        } else
        {
            pennies += cents;
            cents = 0;
        }
    }
    
    printf("%i\n", (quarters + dimes + nickles + pennies));

}
