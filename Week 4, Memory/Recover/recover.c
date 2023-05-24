#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 512
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open input file
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        printf("Could not open %s\n", argv[1]);
        return 1;
    }

    // Array for storing filename
    char *filename = malloc(sizeof(char) * 8);
    if (filename == NULL)
    {
        printf("Failed to allocate memory\n");
        fclose(inptr);
        return 2;
    }

    BYTE buffer[BLOCK_SIZE];
    int count = 0;
    FILE *img = NULL;

    // Read 512 bytes at a time and write to buffer untill EOF
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, inptr) == BLOCK_SIZE)
    {
        // Checking the block in buffer is starting of a JPEG or not
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If img is not NULL, it means some JPEG is in img and since we found new JPEG, we need to fclose and create a new file
            if (img != NULL)
            {
                fclose(img);
            }

            // Creating new file and writing buffer to it
            sprintf(filename, "%03i.jpg", count);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                printf("Could not create file\n");
                fclose(inptr);
                free(filename);
                return 3;
            }
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, img);

            count++;
        }
        else
        {
            if (img != NULL)
            {
                fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, img);
            }
        }
    }

    // If there isn't any JPEG in memorycard, img will still be NULL
    if (img != NULL)
    {
        fclose(img);
    }

    fclose(inptr);
    free(filename);
    return 0;
}