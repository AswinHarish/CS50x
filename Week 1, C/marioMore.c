#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //  Variable declaration
    int size, n = 1;

    // Getting user input
    do
    {
        do
        {
            size = get_int("Height: ");
        }
        while (size > 8);
    }
    while (size < 1);

    // Printing pattern based on user input
    for (int i = size; i >= 1; i--)
    {
        for (int j = 0; j < i - 1; j++)
        {
            printf(" ");
        }
        for (int k = 0; k < n; k++)
        {
            printf("#");
        }
        printf("  ");
        for (int l = 0; l < n; l++)
        {
            printf("#");
        }
        // Printing new line
        printf("\n");
        n++;
    }
}
