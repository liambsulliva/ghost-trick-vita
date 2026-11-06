#include "crc32.h"

#define CRC32_POLYNOMIAL 0xEDB88320U
#define CRC32_TABLE_SIZE 256
#define BITS_PER_BYTE 8

static unsigned int table[CRC32_TABLE_SIZE];
static int table_is_ready;

static void fill_table(void)
{
    unsigned int value;
    unsigned int index;
    int bit;

    for (index = 0; index < CRC32_TABLE_SIZE; index++) {
        value = index;
        for (bit = 0; bit < BITS_PER_BYTE; bit++) {
            value = (value & 1) ? CRC32_POLYNOMIAL ^ (value >> 1) : value >> 1;
        }
        table[index] = value;
    }
    table_is_ready = 1;
}

unsigned int crc32_add(unsigned int crc, const unsigned char *data, unsigned int size)
{
    unsigned int index;

    if (!table_is_ready) {
        fill_table();
    }
    crc = crc ^ 0xFFFFFFFFU;
    for (index = 0; index < size; index++) {
        crc = table[(crc ^ data[index]) & 0xFF] ^ (crc >> BITS_PER_BYTE);
    }
    return crc ^ 0xFFFFFFFFU;
}
