#include "../include/queens.h"
#include <stdlib.h>

int     lines_under_attack(Cell **table, int rows, int columns, int size)
{
    int i;

    i = 1;
    while (rows + i < size && columns + i < size)
    {
        if (table[rows + i][columns + i].figure == 'Q')
            return (0);
        i++;
    }
    i = -1;
    while (rows + i >= 0 && columns + i >= 0)
    {
        if (table[rows + i][columns + i].figure == 'Q')
            return (0);
        i--;
    }
    i = 1;
    while (rows + i < size && columns - i >= 0)
    {
        if (table[rows + i][columns - i].figure == 'Q')
            return (0);
        i++;
    }
    i = -1;
    while (rows + i >= 0 && columns - i < size)
    {
        if (table[rows + i][columns - i].figure == 'Q')
            return (0);
        i--;
    }
    i = 0;
    while (i < size)
    {
        if (table[rows][i].figure == 'Q')
            return (0);
        i++;
    }
    return (1);
}

int   recursive_search(Cell **table, int size, int column)
{
    int i;

    i = 0;
    if (column >= size)
        exit (0);
    while (i < size)
    {
        if (table[i][column].figure == 'Q')
            recursive_search(table, size, column + 1);
        i++;
    }
    i = 0;
    while (i < size)
    {
        if (lines_under_attack(table, i, column, size))
        {
            table[i][column].figure = 'Q';
            if (recursive_search(table, size, column + 1))   
                return (1); 
            table[i][column].figure = 'E';
        }
        i++;
    }
    return (0);
}

void    place_queens(Cell **table, int size)
{
    recursive_search(table, size, 0);
}
