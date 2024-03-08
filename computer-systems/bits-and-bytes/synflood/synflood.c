#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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
    uint32_t llh;
} pcap_header_t;

typedef struct Per_Packet_Header
{
    uint32_t ts_seconds;
    // microseconds or nanoseconds, depending on pcap header magic number
    uint32_t ts_part;
    uint32_t packet_len;
    uint32_t untrunc_packet_len;
} per_packet_header_t;

int main(int argc, char **argv)
{
    // Load the pcap file
    FILE *pcap_file = fopen("csprimer-files/synflood.pcap", "rb");
    if (pcap_file == NULL)
    {
        perror("Unable to open pcap file");
        exit(EXIT_FAILURE);
    }

    // Load and validate the file header
    pcap_header_t pcap_header;
    size_t read_count = fread(&pcap_header, sizeof(pcap_header), 1, pcap_file);
    if (read_count != 1)
    {
        if (ferror(pcap_file))
        {
            perror("Invalid pcap file");
        }
        else
        {
            fprintf(stderr, "Invalid pcap file.");
        }
        
        exit(EXIT_FAILURE);
    }
    // Test if our machine is a different endianness from the machine that produced the savefile
    // We don't care about time precision here (I think?), so ignore case of a1b23c4d (see man pcap-savefile)
    uint32_t host_magic = 0xa1b2c3d4;
    bool swap_bytes = false;
    if (pcap_header.magic == bswap_32(host_magic))
    {
        swap_bytes = true;
        pcap_header.major_version = bswap_16(pcap_header.major_version);
        pcap_header.minor_version = bswap_16(pcap_header.minor_version);
        pcap_header.tz_offset  = bswap_32(pcap_header.tz_offset);
        pcap_header.ts_accuracy = bswap_32(pcap_header.ts_accuracy);
        pcap_header.snapshot_len = bswap_32(pcap_header.snapshot_len);
        pcap_header.llh = bswap_32(pcap_header.llh);
    } 
    else if (pcap_header.magic != host_magic) 
    {
        fprintf(stderr, "Invalid pcap file.");
        exit(EXIT_FAILURE);
    }

    // Read the next packet, starting with the per-packet header
    per_packet_header_t per_packet_header;
    read_count = fread(&per_packet_header, sizeof(per_packet_header_t), 1, pcap_file);
    if (read_count != 1) 
    {
        perror("Error while reading pcap packet header");
        exit(EXIT_FAILURE);
    }

    // Note: we are ignoring truncated packets with the assumption that the snapshot_len is large enough to house the TCP header data
    uint8_t *buffer = malloc(per_packet_header.packet_len);
    if (buffer == NULL)
    {
        perror("Could not allocate memory for packet data");
        exit(EXIT_FAILURE);
    }

    read_count = fread(buffer, sizeof(buffer), 1, pcap_file);
    if (read_count != 1 && ferror(pcap_file))
    {
        perror("Error while reading pcap packet");
        exit(EXIT_FAILURE);
    }
    uint32_t proto = *(uint32_t*)buffer;
    if (swap_bytes) 
    {
        proto = bswap_32(proto);
    }
    if (proto != 2)
    {
        perror("Unsupported protocol. Only IPv4 packet is supported.");
        exit(EXIT_FAILURE);
    }

    fclose(pcap_file);
    return 0;
}