#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);

int main(void)
{
    // Get user input as a string
    string message = get_string("Message: ");
    int a, mLen = strlen(message), bin[BITS_IN_BYTE];

    // Converting char to decimal
    for (int i = 0; i < mLen; i++)
    {
        a = message[i];
        // Decimal to binary
        for (int j = 0; j < BITS_IN_BYTE; j++)
        {
            bin[j] = a % 2;
            a /= 2;
        }
        for (int k = 7; k >= 0; k--)
        {
            print_bulb(bin[k]);
        }
        printf("\n");
    }
}

// Function to print bulbs bit by bit
void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}
