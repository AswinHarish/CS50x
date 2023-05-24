#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Getting card number from user
    long cardNum = get_long("Number: ");

    // Counting card length
    int length = 0;
    long cardLen = cardNum;
    while (cardLen > 0)
    {
        cardLen /= 10;
        length++;
    }

    // Checking validity of length
    if (length != 13 && length != 15 && length != 16)
    {
        printf("INVALID\n");
        return 0;
    }

    // Luhn’s Algorithm to check card validity
    int sumOne = 0, sumTwo = 0;
    long cardOne = cardNum;

    do
    {
        // Remove last digit and add to sumOne
        int lastDigit = cardOne % 10;
        cardOne /= 10;
        sumOne += lastDigit;

        // Remove second last digit
        int secondLastDigit = cardOne % 10;
        cardOne /= 10;

        // Double second last digit and add its digits to sumTwo
        secondLastDigit *= 2;
        int digitOne = secondLastDigit / 10;
        int digitTwo = secondLastDigit % 10;
        sumTwo += digitOne + digitTwo;
    }
    while (cardOne > 0);

    int result = sumOne + sumTwo;
    if (result % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // Find starting 2 digits
    long cardTwo = cardNum;
    do
    {
        cardTwo /= 10;
    }
    while (cardTwo >= 100);

    // Checking card type
    if ((cardTwo / 10 == 3) && (cardTwo % 10 == 4 || cardTwo % 10 == 7))
    {
        printf("AMEX\n");
    }
    else if ((cardTwo / 10 == 5) && (cardTwo % 10 < 6) && (cardTwo % 10 > 0))
    {
        printf("MASTERCARD\n");
    }
    else if ((cardTwo / 10 == 4))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }

}
