#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    // Change all black pixels to a color of your choosing
    for (int i = 0; i < height; i++) // Iterates over height of bmp
    {
        for (int j = 0; j < width; j++) // Iterates over weidth of bmp
        {
            RGBTRIPLE pixel = image[i][j];
            // Changing color if pixel is black
            if (pixel.rgbtBlue == 0 && pixel.rgbtGreen == 0 && pixel.rgbtRed == 0)
            {
                pixel.rgbtBlue = 217;
                pixel.rgbtGreen = 128;
                pixel.rgbtRed = 50;
                image[i][j] = pixel;
            }
        }
    }
}
