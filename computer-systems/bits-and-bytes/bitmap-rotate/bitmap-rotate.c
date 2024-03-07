#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: bitmap-rotate <bitmap file>.bmp\n");
        exit(EXIT_FAILURE);
    }

    FILE *input_file = fopen(argv[1], "rb");
    if (input_file == NULL)
    {
        perror("Unable to open input file");
        exit(EXIT_FAILURE);
    }

    uint8_t header[14];
    size_t ret_code = fread(&header, sizeof header, 1, input_file);
    if (ret_code < 1)
    {
        perror("Unable to read bitmap header file");
        exit(EXIT_FAILURE);
    }

    uint32_t file_size = *(uint32_t *)(header + 2);
    uint8_t *input_buffer = malloc(file_size);
    if (input_buffer == NULL) 
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    rewind(input_file);
    fread(input_buffer, file_size, 1, input_file);

    uint8_t *output_buffer = malloc(file_size);
    if (output_buffer == NULL) 
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    
    memcpy(output_buffer, input_buffer, file_size);

    uint32_t data_offset = *(uint32_t *)(input_buffer + 10 * sizeof(uint8_t));
    uint32_t image_width = *(uint32_t *)(input_buffer + 18 * sizeof(uint8_t));
    uint32_t image_height = *(uint32_t *)(input_buffer + 22 * sizeof(uint8_t));
    uint16_t bits_per_pixel = *(uint16_t *)(input_buffer + 28 * sizeof(uint8_t));
    uint32_t row_size = (image_width * bits_per_pixel / 32) * 4;

    for (uint32_t row = 0; row < image_height; row++) 
    {
        uint8_t *input_pixel = input_buffer + data_offset + row * row_size;

        // Assuming 3 bytes for now (i.e. 24 bits per pixel)
        uint8_t *output_pixel = output_buffer + data_offset + row_size * (image_height - 1) + row * 3;
        for (uint32_t col = 0; col < image_width; col++) 
        {
            // Assuming 3 bytes for now (i.e. 24 bits per pixel)
            *output_pixel = *input_pixel; // r
            *(output_pixel + 1) = *(input_pixel + 1); // g
            *(output_pixel + 2) = *(input_pixel + 2); // b
            output_pixel -= row_size;
            input_pixel += 3;
        }
    }

    FILE *output_file = fopen("./csprimer-files/output_file.bmp", "wb");
    if (output_file == NULL)
    {
        perror("Unable to open output file to write to");
        exit(EXIT_FAILURE);
    }
    fwrite(output_buffer, file_size, 1, output_file);
    fclose(output_file);
    fclose(input_file);
    return 0;
}