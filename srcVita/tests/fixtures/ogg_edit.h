#ifndef OGG_EDIT_H
#define OGG_EDIT_H

#include "ogg_pages.h"

static void ogg_edit_refresh_crcs(unsigned char *data, unsigned int size)
{
    unsigned int offset;
    unsigned int page_size;
    unsigned int crc;
    int byte;

    offset = 0;
    while (offset < size) {
        page_size = ogg_page_size(&data[offset], size - offset);
        crc = ogg_page_crc(&data[offset], page_size);
        for (byte = 0; byte < OGG_PAGE_CRC_BYTES; byte++) {
            data[offset + OGG_PAGE_CRC_OFFSET + (unsigned int)byte] = (unsigned char)(crc >> (8 * byte));
        }
        offset = offset + page_size;
    }
}

#endif
