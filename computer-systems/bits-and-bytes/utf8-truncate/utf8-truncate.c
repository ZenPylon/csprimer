#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>

static inline uint16_t bswap_16(uint16_t a)
{
    return (a & 0xFF00) >> 8 | (a & 0x00FF) << 8;
}

static inline uint32_t bswap_32(uint32_t a)
{
    return (a & 0xFF000000) >> 24 | (a & 0x00FF0000) >> 8 | (a & 0x0000FF00) << 8 | (a & 0x000000FF) << 24;
}

int main(int argc, char **argv)
{
    // Open the cases file
    const char *cases_filename = "./csprimer-files/cases";
    FILE *cases_file = fopen(cases_filename, "rb");

    if (cases_file == NULL)
    {
        perror("Unable to open cases file");
        exit(EXIT_FAILURE);
    }
    
    // Find out how many bytes to read from the cases file
    struct stat file_info;
    int stat_return = stat(cases_filename, &file_info);
    if (stat_return == -1)
    {
        perror("Unable to load file information for cases");
        exit(EXIT_FAILURE);
    }

    // Allocate input and output buffers
    // Since we're truncating, we can guarantee that the output buffer will be at most equal 
    // in size to the input buffer
    unsigned char *cases_buffer = malloc(file_info.st_size );
    unsigned char *output_buffer = malloc(file_info.st_size);
    if (cases_buffer == NULL || output_buffer == NULL) 
    {
        perror("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }
    
    // The load the cases file data into memory
    fread(cases_buffer, file_info.st_size, 1, cases_file);

    uint8_t linechar_counter = 0; // how far past the first byte we are on this particular line
    uint8_t trunc_len;
    unsigned char *cases_ptr = cases_buffer;
    unsigned char *output_ptr = output_buffer;

    while (cases_ptr - cases_buffer < file_info.st_size) 
    {
        if (linechar_counter == 0) 
        {
            trunc_len = (uint8_t)*cases_ptr;
            // Don't include the truncate length in the output buffer
            cases_ptr++;
        }
        // Figure out how long this particular character is, and 
        uint8_t num_char_bytes = 0;
        unsigned char start_byte = *cases_ptr;

        // One-byte character
        if (!(start_byte & 0x80))
        {
            num_char_bytes = 1;
        }
        // Two-byte character
        else if ((start_byte >> 5) == 0x06)
        {
            num_char_bytes = 2;
        }
        // Three-byte character
        else if ((start_byte >> 4) == 0x0d)
        {
            num_char_bytes = 3;
        }
        // Four-byte character
        else if ((start_byte >> 3) == 0x1e)
        {
            num_char_bytes = 4;
        }
        else 
        {
            fprintf(stderr, "Encountered invalid unicode character.\n");
            exit(EXIT_FAILURE);
        }

        // If this character would exceed the truncate length, don't copy the bytes from the cases file.
        // Instead, read through the chars on this line until we find the newline, and start over (by setting linechar_counter = 0)
        if (linechar_counter + num_char_bytes > trunc_len || *cases_ptr == '\n')
        { 
            while (*cases_ptr != '\n') 
            {
                cases_ptr++;
            }
            *output_ptr = '\n';
            cases_ptr++;
            output_ptr++;
            linechar_counter = 0;
            continue;
        }
        // Otherwise, copy over each byte from the cases buffer into the output buffer
        for (int i = 0; i < num_char_bytes; i++)
        {
            *output_ptr = *cases_ptr;
            output_ptr++;
            cases_ptr++;
            linechar_counter++;
        }
    }
    *output_ptr = '\0';
    FILE *output_file = fopen("csprimer-files/output", "wb");
    fwrite(output_buffer, output_ptr - output_buffer, 1, output_file);

    free(cases_buffer);
    free(output_buffer);

    return 0;
}