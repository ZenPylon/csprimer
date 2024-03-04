#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>

// Arbitrarily limit line length (not realistic in prod)
const size_t MAX_LENGTH = 256;

// Max number of chars in hex string
const size_t MAX_HEX_CHARS = 8;

// Arbitrary rgba char limit
const size_t MAX_RGBA_CHARS = 30;

typedef struct CSSProperty {
    char property[30];
    char value[30];
} css_property;

// TODO - create struct for property value

// Extracts a hexadecimal string following a `color:` property
// Returns NULL if the line does not contain "color:"
// Does not validate the hex value following "color:"
// Example: `background-color: #fff` returns "fff"
// Caller is responsible for freeing the returned value
char *extract_hex_chars(char *line)
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
        if (match_c - match == match_len)
        {
            has_color = true;
            break;
        }
    }
    if (has_color)
    {
        hex_chars = malloc(MAX_HEX_CHARS);
        char *hex_char = hex_chars;

        while (*curr_c != '\0' && *curr_c != ';' && hex_char - hex_chars < MAX_HEX_CHARS)
        {
            if (*curr_c == '#' || *curr_c == ' ')
            {
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

// Converts a hex char like 'c' to the value 10
// This case-insensitive, so F and f are valid
// Returns -1 if hex is invalid
int8_t hex_char_to_dec(char hex)
{
    // For 0-9
    if ((int8_t)hex >= 48 && hex < 58)
    {
        return hex - 48;
    }
    else if (hex >= 65 && hex < 71)
    {
        return hex - 55;
    }
    else if (hex >= 97 && hex < 103)
    {
        return hex - 87;
    }
    else
    {
        return -1;
    }
}

// Returns a hex color string in rgb or rgba() format
// Caller is responsible for freeing the returned memory
char *hex_chars_to_rgba(char *hex_chars)
{
    size_t num_chars = strlen(hex_chars);
    char *rgba_chars = (char*)malloc(MAX_RGBA_CHARS);

    // if (num_chars == 3)
    // {
    //     int8_t r = hex_chars[0];
    //     int8_t g = hex_chars[0];
    //     int8_t b = hex_chars[0];
    //     sprintf(rgba_chars, "rgb(%d)", r, g, b);
    // }
    if (num_chars == 6) {
        uint8_t r = (uint8_t)(hex_char_to_dec(hex_chars[0]) << 4 & hex_char_to_dec(hex_chars[1]));
        uint8_t g = (uint8_t)(hex_char_to_dec(hex_chars[2]) << 4 & hex_char_to_dec(hex_chars[3]));
        uint8_t b = (uint8_t)(hex_char_to_dec(hex_chars[4]) << 4 & hex_char_to_dec(hex_chars[5]));
        sprintf(rgba_chars, "rgb(%d, %d, %d)", r, g, b);
    }
    return rgba_chars;
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
        fprintf(stderr, "Unable to open CSS File %s. Please double-check the path and filename.\n", argv[1]);
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
    char file_line[MAX_LENGTH];
    while (fgets(file_line, MAX_LENGTH, input_file) != NULL)
    {
        char *hex_chars = extract_hex_chars(file_line);
        if (hex_chars == NULL)
        {
            fprintf(output_file, file_line);
            continue;
        }
        else
        {
            fprintf(output_file, hex_chars_to_rgba(hex_chars));
            free(hex_chars);
        }
    }

    // Replace any hexadecimal color values with rgb
    free(output_filename);
    fclose(input_file);
    fclose(output_file);
    return 0;
}