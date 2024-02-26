#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int ival, index;
    char c;

    while ((c = getc(stdin)))
    {
        if (isdigit(c))
        {
            ival = atoi(&c);
            for (index = 0; index < ival; index++)
            {
                putc('\x07', stdout);
            }
        }
    }
}