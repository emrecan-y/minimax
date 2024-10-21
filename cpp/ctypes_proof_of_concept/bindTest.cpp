#include <iostream>
#include <string>

void print2dArr(int y, int x, char *arrayBegin)
{
    int i, j;
    for (i = 0; i < y; i++)
    {
        for (j = 0; j < x; j++)
        {
            printf("%c ", *(arrayBegin + i * x + j));
        }
        printf("\n");
    }
}

extern "C"
{
    void print2dArray(int y, int x, char *array_begin) { print2dArr(y, x, array_begin); }
}