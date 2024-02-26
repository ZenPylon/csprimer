#include <assert.h>
#include <stdio.h>

int bitcount(int input)
{
    int count = 0;
    int input_bitsize = sizeof(int) * 8;

    for (int i = 0; i < input_bitsize; i++)
    {
        count += (input >> i) & 1;
    }
    return count;
}

int main()
{
    assert(bitcount(0) == 0);
    assert(bitcount(1) == 1);
    assert(bitcount(3) == 2);
    assert(bitcount(8) == 1);
    // harder case:
    assert(bitcount(0xffffffff) == 32);
    printf("OK\n");
}
