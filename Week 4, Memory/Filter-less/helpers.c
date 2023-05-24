#include "helpers.h"
#include <math.h>

// Convert image to grayscale 
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Changing each pixel to average of it's RGB value
            int avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = image[i][j].rgbtGreen = image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculating sepia formula for each pixel
            int red = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            int green = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int blue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);

            // Changing pixels to get sepia filter
            image[i][j].rgbtRed = (red > 255) ? 255 : red;
            image[i][j].rgbtGreen = (green > 255) ? 255 : green;
            image[i][j].rgbtBlue = (blue > 255) ? 255 : blue;

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterating through height
    for (int i = 0; i < height; i++)
    {
        // Swapping Pixels
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE pixel = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = pixel;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copying image to temp (pixel by pixel)
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // for finding blur value of each pixel
            int blurRed, blurGreen, blurBlue;
            blurRed = blurGreen = blurBlue = 0;

            // Count of valid pixels
            float count = 0.00;

            // Get neighbouring pixels (x and y are coordinates of matrix)
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    // finds the value of each pixel in coordinates
                    int newx = i + x;
                    int newy = j + y;

                    // Checking validity of neighbouring pixels
                    if (newx >= 0 && newy >= 0 && newx <= (height - 1) && newy <= (width - 1))
                    {
                        blurRed += temp[newx][newy].rgbtRed;
                        blurGreen += temp[newx][newy].rgbtGreen;
                        blurBlue += temp[newx][newy].rgbtBlue;

                        count++;
                    }
                }

                // Finding average and blurring pixel
                image[i][j].rgbtRed = round(blurRed / count);
                image[i][j].rgbtGreen = round(blurGreen / count);
                image[i][j].rgbtBlue = round(blurBlue / count);
            }
        }
    }

    return;
}
