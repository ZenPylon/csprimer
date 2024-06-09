#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const int max_msg_bytes = 6;

// conceal supports a maximum msg length of 6 bytes
// We're assuming that msg is not entirely null characters (which would cause conceal to return a value of infinity
double conceal(unsigned char *msg)
{
    // The least significant 52 bits (6 bytes + 4 bits) are the coefficient (coefficient of the float)
    // Note that we're arbitrarily setting the sign bit to 1 in this case
    uint64_t nan_mask = 0x7FF0000000000000;
    unsigned char *ptr = msg;
    int8_t count;

    // Go through all characters in msg until encountering a null-terminator or the max msg length is reached.
    for (count = max_msg_bytes - 1; count >= 0; count--)
    {
        unsigned char curr_char = *ptr;
        if (curr_char == '\0')
        {
            break;
        }
        // Put the first character in the most significant byte of the double
        nan_mask |= ((uint64_t)curr_char) << (8 * count);
        ptr++;
    }

    // If there are over six characters (the max length), make sure the seventh is a null-terminator.
    // Otherwise, the the caller tried to pass a string that's too large. Let them know.
    if (count < 0 && msg[6] != '\0')
    {
        fprintf(stderr, "Invalid message: too many characters");
        exit(EXIT_FAILURE);
    }
    return *((double *)&nan_mask);
}

unsigned char *extract(double num)
{
    if (!isnan(num))
    {
        fprintf(stderr, "Trying to extract message from a double that isn't a NaN");
        exit(EXIT_FAILURE);
    }
    // Allocate an additional byte for the null terminator
    unsigned char *msg = calloc(max_msg_bytes + 1, 1);
    unsigned char *ptr = msg;

    // (Alternatively, we could swap the bytes depending on the architecture, but this avoids the need to)
    #if __ORDER_LITTLE_ENDIAN__
        unsigned char* num_as_bytes = ((unsigned char*) (double *)&num) + max_msg_bytes - 1;
        for (int8_t i = max_msg_bytes - 1; i >= 0; i--)
        {
            *ptr = *num_as_bytes;
            num_as_bytes--;
            ptr++;
        }
    #else
        // +2 to skip the first two bytes of num, which are not used to encode anything
        unsigned char* num_as_bytes = ((unsigned char*) (double *)&num) + 2;
        for (uint8_t i = 0; i < max_msg_bytes; i++)
        {
            *ptr = *num_as_bytes;
            ptr++;
            num_as_bytes++;
        }
    #endif

    return msg;
}

int main(int argc, char **argv)
{
    double test = conceal("sneaky!");
    unsigned char *msg = extract(test);
    printf("Hidden message was %s", msg);
}