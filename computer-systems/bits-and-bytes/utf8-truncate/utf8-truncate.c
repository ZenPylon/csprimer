#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

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
    // Read the cases file
    const char *cases_filename = "./csprimer-files/cases";
    FILE *cases_file = fopen(cases_filename, "rb");

    if (cases_file == NULL)
    {
        perror("Unable to open cases file");
        exit(EXIT_FAILURE);
    }
    struct stat file_info;
    stat(cases_filename, &file_info);
    printf("File is %lld bytes long\n", file_info.st_size);

    // fread(&cases_buffer, BUFFER_SIZE, 1, cases_file);

    // char *output_buffer = malloc(BUFFER_SIZE);
    // int output = 0;


    // FILE *output_file = fopen("output", "wb");
    // fwrite(output_buffer, 0, 1, output_file);

    return 0;
}