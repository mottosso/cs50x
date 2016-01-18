/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */

#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

#include "helpers.h"

bool iterativeSearch(int value, int values[], int n);

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{

    return iterativeSearch(value, values, n);
}

/**
 * Binary search, as an iterative function
*/
bool iterativeSearch(int value, int values[], int n)
{
    int min = 0, max = n;
    for (int i = 0; i < n; i++)
    {
        int middle = min + (max - min) / 2;

        if (value == values[middle])
        {
            return true;
        }
        else if (value > values[middle])
        {
            min = middle;
        }
        else
        {
            max = middle;
        }
    }
    return false;
}

/**
 * The bubble sort algorithm
 *
*/
void bubbleSort(int values[], int n)
{
    int tmp;
    for (int i = 0; i < (n - 1); i++)
    {
        for (int j = 0; j < (n - 1 - i); j++)
        {
            if (values[j] > values[j + 1])
            {
                tmp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = tmp;
            }
        }
    }
    return;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    bubbleSort(values, n);
    return;
}
