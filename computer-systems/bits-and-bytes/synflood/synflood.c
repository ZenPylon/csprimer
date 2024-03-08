#include <stdlib.h>
#include <stdio.h>

static inline uint16_t bswap_16(uint16_t a)
{
    return (a & 0xFF00) >> 8 | (a & 0x00FF) << 8;
}

static inline uint32_t bswap_32(uint32_t a)
{
    return (a & 0xFF000000) >> 24 | (a & 0x00FF0000) >> 8 | (a & 0x0000FF00) << 8 | (a & 0x000000FF) << 24;
}

typedef struct Pcap_Header
{
    uint32_t magic;
    uint16_t major_version;
    uint16_t minor_version;
    uint32_t tz_offset;
    uint32_t ts_accuracy;
    uint32_t snapshot_len;
    uint32_t link_layer_header_type;
} pcap_header_t;

int main(int argc, char **argv)
{
    FILE *pcap_file = fopen("csprimer-files/synflood.pcap", "rb");
    if (pcap_file == NULL)
    {
        perror("Unable to open pcap file");
        exit(EXIT_FAILURE);
    }

    pcap_header_t header;
    size_t read_count = fread(&header, sizeof(header), 1, pcap_file);
    if (read_count != 1)
    {
        perror("Not a valid pcap header");
        exit(EXIT_FAILURE);
    }

    // little ending magic string
    uint32_t host_magic = 0xa1b2c3d4;
    return 0;
}