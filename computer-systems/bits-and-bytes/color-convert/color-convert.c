#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>

// Arbitrarily limit line length
const size_t MAX_LENGTH = 256;

// Max number of chars in hex string
const size_t MAX_HEX_CHARS = 8;

// Extracts a hexadecimal string following a `color:` property
// Returns NULL if the line does not contain "color:"
// Does not validate the hex value following "color:"
// Example: `background-color: #fff` returns "fff"
// Caller is responsible for freeing the returned value
char *extract_hex_chars(const char *line)
{
    char *match = "color:";
    size_t match_len = strlen(match);
    
    char *match_c = match;
    char *curr_c = line;

    size_t hex_start;
    size_t hex_end;
    char *hex_chars = NULL;
    bool has_color = false;

    while (*curr_c != '\0')
    {
        if (*curr_c == *match_c)
        {
            match_c++;
        }
        else
        {
            // Reset the matching character to test against if this character is not the next in the sequence of "color"
            match_c = match;
        }
            
        curr_c++;
        // We've got "color:"
        if (match_c - match == match_len) {
            has_color = true;
            break;
        }
    }
    if (has_color) {
        hex_chars = malloc(MAX_HEX_CHARS);
        char *hex_char = hex_chars;

        while (*curr_c != '\0' && *curr_c != ';') {
            if (*curr_c == '#' || *curr_c == ' ') {
                curr_c++;
                continue;
            }
            *hex_char = *curr_c;
            hex_char++;
            curr_c++;
        }
    }
    
    return hex_chars;
}

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
    char *file_line[MAX_LENGTH];
    while (fgets(file_line, MAX_LENGTH, input_file) != NULL)
    {
        char *hex_chars = extract_hex_chars(file_line);
        if (hex_chars == NULL) {
            fprintf(output_file, file_line);    
            continue;
        } else {
            free(hex_chars);
        }
    }
    

    // Replace any hexadecimal color values with rgb
    free(output_filename);
    fclose(input_file);
    fclose(output_file);
    return 0;
}