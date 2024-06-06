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
    printf("File is %lld bytes long\n", file_info.st_size);

    // Allocate input and output buffers
    // Since we're truncating, we can guarantee that the output buffer will be at most equal 
    // in size to the input buffer
    char *cases_buffer = malloc(file_info.st_size );
    char *output_buffer = malloc(file_info.st_size);
    if (cases_buffer == NULL || output_buffer == NULL) 
    {
        perror("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }
    
    // The load the cases file data into memory
    fread(cases_buffer, file_info.st_size, 1, cases_file);
    printf("%s", cases_buffer);

    uint8_t linechar_counter = 0; // how far past the first byte we are on this particular line
    uint8_t trunc_len;
    char *cases_ptr = cases_buffer;
    char *output_ptr = output_buffer;

    while (cases_ptr - cases_buffer < file_info.st_size) 
    {
        if (linechar_counter == 0) 
        {
            trunc_len = (uint8_t)*cases_ptr;
            // Don't include the truncate length in the output buffer
            cases_ptr++;
        }
        
        printf("Trunc length is %d\n", trunc_len);
        // NOTE - we're assuming that the input is well formed i.e. that each line has at least the specified trunc_len
        if (linechar_counter >= trunc_len || *cases_ptr == '\n')
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
        printf("Copying letter %c\n", *cases_ptr);
        *output_ptr = *cases_ptr;
        output_ptr++;
        cases_ptr++;
        linechar_counter++;
    }
    *output_ptr = '\0';
    FILE *output_file = fopen("csprimer-files/output", "wb");
    fwrite(output_buffer, output_ptr - output_buffer, 1, output_file);

    free(cases_buffer);
    free(output_buffer);

    return 0;
}