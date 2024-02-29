#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

int main(int argc, char **argv)
{
    // Validate program input
    if (argc != 2)
    {
        fprintf(stderr, "Usage: color-convert css_file.css");
        exit(1);
    }
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        fprintf(stderr, "Unable to open CSS File. Please double-check the path and filename.\n");
        exit(1);
    }

    // Create a new file based on the existing filename, plus a suffix
    int i = 0;
    const char *suffix = "_output.css";
    while (argv[1][i] != '.' && argv[1][i] != '\0')
    {
        i++;
    }

    size_t output_filename_len = i + strlen(suffix) + 1;
    char *output_filename = malloc(output_filename_len);
    memcpy(output_filename, argv[1], i);
    memcpy(output_filename + i, suffix, strlen(suffix));
    output_filename[output_filename_len] = '\0';

    FILE *output_file = fopen(output_filename, "w");

    if (output_file == NULL)
    {
        perror("ERROR:");
        exit(1);
    }
    fprintf(output_file, "Hello, file");

    // Replace any hexadecimal color values with rgb
    free(output_filename);
    fclose(input_file);
    fclose(output_file);
    return 0;
}