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
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define ntohs(x) bswap_16(x)
#define ntohl(x) bswap_32(x)
#else
#define ntohs(x) (x)
#define ntohl(x) (x)
#endif

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
    
    // Parse all packets in the savefile
    uint32_t num_packets = 0;
    uint32_t num_port_80 = 0;
    uint32_t num_syns = 0;
    uint32_t num_acks = 0;

    while (true) 
    {
        // Read the next packet, starting with the per-packet header
        per_packet_header_t per_packet_header;
        read_count = fread(&per_packet_header, sizeof(per_packet_header_t), 1, pcap_file);
        if (read_count != 1 && feof(pcap_file))
        {
            break;
        }
        if (read_count != 1 && ferror(pcap_file)) 
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

        read_count = fread(buffer, per_packet_header.packet_len, 1, pcap_file);
        if (read_count != 1)
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
            fprintf(stderr, "Unsupported protocol. Only IPv4 packet is supported.");
            exit(EXIT_FAILURE);
        }
        // loopback interface protocol field is 4 bytes
        uint8_t *ip_header = buffer + 4; 
        uint8_t ihl = 0x0f & *ip_header;
        if (*(ip_header + 9) != 6)
        {
            fprintf(stderr, "Unsupported protocol. Only accepts TCP (protocol 6)");
            exit(EXIT_FAILURE);
        }
        // Skip ahead for each ip header DWORD
        uint16_t *source_port = (uint16_t *) (ip_header + ihl * 4);
        
        // ip_header fields appear to be in big-endian (?)
        uint16_t *dest_port = source_port + 1;

        // printf("%d --> %d\n", bswap_16(*source_port), bswap_16(*dest_port));
        uint8_t flags = *(((uint8_t *)source_port) + 13);

        // Ack bit
        if (bswap_16(*source_port) == 80 && flags & 0x10)
        {   
            num_acks++;
        }
        // Syn bit
        if (bswap_16(*dest_port) == 80 && flags & 0x02)
        {
            num_syns++;
        }
        num_packets++;
    }
    

    printf("Parsed a total of %d packets", num_packets);
    printf("There were %d syn packets on port 80, but only %d ack packets", num_syns, num_acks);

    fclose(pcap_file);
    return 0;
}