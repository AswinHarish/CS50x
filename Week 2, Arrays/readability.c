#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Getting user input and passing it to functions
    string text = get_string("Text: ");
    int letter = count_letters(text);
    int words = count_words(text);
    int sentence = count_sentences(text);

    // Finding L & S
    float L = ((float)letter / (float)words) * 100;
    float S = ((float)sentence / (float)words) * 100;

    // Calculating Coleman-Liau index
    float index = 0.0588 * L - 0.296 * S - 15.8;
    int grade = round(index);

    // Printing the grade
    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}


// Function to find letter count
int count_letters(string text)
{
    int len = 0;
    // Counting letters
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            len++;
        }
    }
    return len;
}

// Function to find word count
int count_words(string text)
{
    int len = 0;
    // Counting words
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == ' ')
        {
            len++;
        }
    }
    return len + 1;
}

// Function to find sentence count
int count_sentences(string text)
{
    int len = 0;
    // Counting sentences
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            len++;
        }
    }
    return len;
}
