#ifndef OGG_PAGES_H
#define OGG_PAGES_H

#define OGG_PAGE_CRC_OFFSET 22
#define OGG_PAGE_CRC_BYTES 4

unsigned int ogg_page_size(const unsigned char *data, unsigned int size);
unsigned int ogg_page_crc(const unsigned char *page, unsigned int page_size);
int ogg_pages_are_intact(const unsigned char *data, unsigned int size);

#endif
