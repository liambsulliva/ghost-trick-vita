#include <stdlib.h>
#include "fixtures/fixture_facts.h"
#include "fixtures/ogg_edit.h"
#include "fixtures/sine_mono_ogg.h"
#include "fixtures/sine_stereo_ogg.h"
#include "ogg_pages.h"
#include "test_support.h"

#define CRC_POLYNOMIAL 0x04C11DB7U
#define PAGE_HEADER_BYTES 27
#define SEGMENT_COUNT_OFFSET 26
#define ALL_BYTE_VALUES 256
#define FULL_SEGMENT_BYTES 255
#define TWO_SEGMENTS 2
#define BYTES_TO_FLIP_ON_ALL_BITS 64

static unsigned int crc_one_bit_at_a_time(const unsigned char *data, unsigned int size)
{
    unsigned int crc;
    unsigned int index;
    int bit;

    crc = 0;
    for (index = 0; index < size; index++) {
        crc = crc ^ ((unsigned int)data[index] << 24);
        for (bit = 0; bit < 8; bit++) {
            crc = (crc & 0x80000000U) ? (crc << 1) ^ CRC_POLYNOMIAL : crc << 1;
        }
    }
    return crc;
}

static unsigned int make_page_with_every_byte_value(unsigned char *page)
{
    unsigned int body_offset;
    unsigned int body_bytes;
    unsigned int index;

    body_offset = PAGE_HEADER_BYTES + TWO_SEGMENTS;
    body_bytes = ALL_BYTE_VALUES;
    memset(page, 0, body_offset);
    memcpy(page, "OggS", 4);
    page[SEGMENT_COUNT_OFFSET] = TWO_SEGMENTS;
    page[PAGE_HEADER_BYTES] = FULL_SEGMENT_BYTES;
    page[PAGE_HEADER_BYTES + 1] = (unsigned char)(body_bytes - FULL_SEGMENT_BYTES);
    for (index = 0; index < body_bytes; index++) {
        page[body_offset + index] = (unsigned char)index;
    }
    return body_offset + body_bytes;
}

static void test_real_encoder_output_is_intact(void)
{
    CHECK(ogg_pages_are_intact(sine_mono_ogg, sizeof(sine_mono_ogg)));
    CHECK(ogg_pages_are_intact(sine_stereo_ogg, sizeof(sine_stereo_ogg)));
}

static void test_the_crc_matches_the_one_bit_at_a_time_definition_for_every_byte_value(void)
{
    static unsigned char page[PAGE_HEADER_BYTES + TWO_SEGMENTS + ALL_BYTE_VALUES];
    unsigned int page_size;

    page_size = make_page_with_every_byte_value(page);
    CHECK_INT(ogg_page_size(page, page_size), page_size);
    CHECK_INT(ogg_page_crc(page, page_size), crc_one_bit_at_a_time(page, page_size));
}

static void test_the_crc_field_itself_counts_as_zero(void)
{
    static unsigned char page[PAGE_HEADER_BYTES + TWO_SEGMENTS + ALL_BYTE_VALUES];
    unsigned int page_size;
    unsigned int crc_with_zero_field;

    page_size = make_page_with_every_byte_value(page);
    crc_with_zero_field = ogg_page_crc(page, page_size);
    page[OGG_PAGE_CRC_OFFSET] = 0x5A;
    CHECK_INT(ogg_page_crc(page, page_size), crc_with_zero_field);
}

static void test_a_page_with_a_refreshed_crc_is_intact(void)
{
    static unsigned char page[PAGE_HEADER_BYTES + TWO_SEGMENTS + ALL_BYTE_VALUES];
    unsigned int page_size;

    page_size = make_page_with_every_byte_value(page);
    CHECK(!ogg_pages_are_intact(page, page_size));
    ogg_edit_refresh_crcs(page, page_size);
    CHECK(ogg_pages_are_intact(page, page_size));
}

static void check_a_flipped_bit_is_noticed(unsigned char *copy, unsigned int size, unsigned int index, int bit)
{
    copy[index] = (unsigned char)(copy[index] ^ (1 << bit));
    CHECK(!ogg_pages_are_intact(copy, size));
    copy[index] = (unsigned char)(copy[index] ^ (1 << bit));
}

static void test_every_flipped_byte_is_noticed(void)
{
    static unsigned char copy[sizeof(sine_mono_ogg)];
    unsigned int index;
    int bit;

    memcpy(copy, sine_mono_ogg, sizeof(copy));
    for (index = 0; index < BYTES_TO_FLIP_ON_ALL_BITS; index++) {
        for (bit = 0; bit < 8; bit++) {
            check_a_flipped_bit_is_noticed(copy, sizeof(copy), index, bit);
        }
    }
    for (index = BYTES_TO_FLIP_ON_ALL_BITS; index < sizeof(copy); index++) {
        check_a_flipped_bit_is_noticed(copy, sizeof(copy), index, index % 8);
    }
    CHECK(ogg_pages_are_intact(copy, sizeof(copy)));
}

static void test_only_a_cut_on_a_page_boundary_is_intact(void)
{
    unsigned char *cut;
    unsigned int size;
    int is_on_a_boundary;

    for (size = 0; size <= sizeof(sine_mono_ogg); size++) {
        cut = malloc(size == 0 ? 1 : size);
        memcpy(cut, sine_mono_ogg, size);
        is_on_a_boundary = size == SINE_MONO_FIRST_PAGE_BYTES || size == SINE_MONO_HEADER_PAGES_BYTES || size == sizeof(sine_mono_ogg);
        CHECK_INT(ogg_pages_are_intact(cut, size), is_on_a_boundary);
        free(cut);
    }
}

static void test_bytes_after_the_last_page_are_not_intact(void)
{
    static unsigned char longer[sizeof(sine_mono_ogg) + 1];

    memcpy(longer, sine_mono_ogg, sizeof(sine_mono_ogg));
    CHECK(!ogg_pages_are_intact(longer, sizeof(longer)));
}

static void test_a_page_size_comes_from_its_lacing_values(void)
{
    CHECK_INT(ogg_page_size(sine_mono_ogg, sizeof(sine_mono_ogg)), SINE_MONO_FIRST_PAGE_BYTES);
    CHECK_INT(ogg_page_size(sine_mono_ogg, SINE_MONO_FIRST_PAGE_BYTES), SINE_MONO_FIRST_PAGE_BYTES);
    CHECK_INT(ogg_page_size(sine_mono_ogg, SINE_MONO_FIRST_PAGE_BYTES - 1), 0);
}

static void test_text_that_is_not_ogg_is_not_a_page(void)
{
    static const unsigned char not_ogg[64] = "this is not an ogg file";

    CHECK_INT(ogg_page_size(not_ogg, sizeof(not_ogg)), 0);
    CHECK(!ogg_pages_are_intact(not_ogg, sizeof(not_ogg)));
}

int main(void)
{
    test_real_encoder_output_is_intact();
    test_the_crc_matches_the_one_bit_at_a_time_definition_for_every_byte_value();
    test_the_crc_field_itself_counts_as_zero();
    test_a_page_with_a_refreshed_crc_is_intact();
    test_every_flipped_byte_is_noticed();
    test_only_a_cut_on_a_page_boundary_is_intact();
    test_bytes_after_the_last_page_are_not_intact();
    test_a_page_size_comes_from_its_lacing_values();
    test_text_that_is_not_ogg_is_not_a_page();
    return test_finish("unit/ogg_pages");
}
