#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool only_digits(string argv);
char rotate(char arg, int key);

int main(int argc, string argv[])
{
    // Validating argc
    if (argc != 2 || only_digits(argv[1]) == false)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Converting and storing key as an integer + Reading plain text
    int key = atoi(argv[1]);
    string text = get_string("plaintext : ");

    // Iterating through plaintext and printing ciphered text
    int len = strlen(text);
    printf("ciphertext: ");
    for (int i = 0; i < len; i++)
    {
        char test = rotate(text[i], key);
        printf("%c", test);
    }
    printf("\n");
}


// Function to check argv elemets are digits or not
bool only_digits(string arg)
{
    for (int i = 0; i < strlen(arg); i++)
    {
        if (isdigit(arg[i]) == false)
        {
            return false;
        }
    }
    return true;
}

// Function to convert plain text to cipher
char rotate(char arg, int key)
{
    if (isalpha(arg))
    {
        if (isupper(arg)) // Ciphering uppercase
        {
            arg = ((int)arg - 65 + key) % 26;
            return (char)arg + 'A';
        }
        else // Ciphering lowercase
        {
            arg = ((int)arg - 97 + key) % 26;
            return (char)arg + 'a';
        }
    }
    else
    {
        return arg;
    }
}
