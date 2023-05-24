#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool check_letter(string arg);
bool duplicate(string arg);
char rotate(char arg, string key);

int main(int argc, string argv[])
{
    // Checking argumnet count
    if (argc != 2)
    {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }
    else if (strlen(argv[1]) != 26 || check_letter(argv[1]) || duplicate(argv[1]))
    {
        printf("Key must contain 26 characters with no duplicates\n");
        return 1;
    }
    // Reading plaintext
    string key = argv[1];
    string text = get_string("plaintext: ");

    // Printing ciphered text
    printf("ciphertext: ");
    for (int i = 0; i < strlen(text); i++)
    {
        printf("%c", rotate(text[i], key));
    }
    printf("\n");
}


// Function to validate key
bool check_letter(string arg)
{
    for (int i = 0; i < strlen(arg); i++)
    {
        if (isdigit(arg[i])) // Checking for digits
        {
            return true;
        }
        else if (ispunct(arg[i])) // Checking for punctuations
        {
            return true;
        }
    }
    return false;
}

// Function to check for duplicate chars
bool duplicate(string arg)
{
    for (int i = 0; i < strlen(arg); i++)
    {
        for (int j = i + 1; j < strlen(arg); j++)
        {
            if (arg[i] == arg[j])
            {
                return true;
            }
        }
    }
    return false;
}

// Function to convert plaintext to ciphertext
char rotate(char arg, string key)
{
    int i;
    if (isalpha(arg))
    {
        if (isupper(arg)) // For ciphering uppercase characters
        {
            i = arg - 'A';
            return toupper(key[i]);
        }
        if (islower(arg)) // For ciphering lowercase characters
        {
            i = arg - 'a';
            return tolower(key[i]);
        }
    }
    return arg;
}
