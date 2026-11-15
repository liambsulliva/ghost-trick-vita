#include <stdlib.h>
#include <string.h>
#include "fakes/fake_platform.h"
#include "file.h"
#include "read_buffer.h"
#include "test_support.h"
#include "texture_file.h"
#include "texture_read.h"

#define UNIT_BYTES 16u
#define PAGE_UNITS (512u * 512u)
#define LITERAL_UNITS 100000u
#define ONE_MIB (1024u * 1024u)
#define PAGE_PATH "textures/page.gtex"

static void write_u32_at(unsigned char *bytes, unsigned int value)
{
    bytes[0] = (unsigned char)(value & 0xffu);
    bytes[1] = (unsigned char)((value >> 8) & 0xffu);
    bytes[2] = (unsigned char)((value >> 16) & 0xffu);
    bytes[3] = (unsigned char)(value >> 24);
}

static void fill_unit(unsigned char *unit, unsigned int index)
{
    unsigned int byte;

    for (byte = 0; byte < UNIT_BYTES; byte++) {
        unit[byte] = (unsigned char)((index >> (8u * (byte % 3u))) + byte);
    }
}

static unsigned int add_match(unsigned char *at, unsigned int length)
{
    write_u32_at(at, 0x80000000u | length);
    write_u32_at(at + 4, 0);
    return 8u;
}

static unsigned int add_packed_page(void)
{
    unsigned char *file;
    unsigned int offset;
    unsigned int index;
    unsigned int left;

    file = malloc(TEXTURE_FILE_HEADER_SIZE + 4u + LITERAL_UNITS * UNIT_BYTES + 256u);
    memcpy(file, "GTEX", 4);
    write_u32_at(&file[4], 2048);
    write_u32_at(&file[8], 2048);
    write_u32_at(&file[12], TEXTURE_FILE_FORMAT_DXT5 | TEXTURE_FILE_PACKED_FLAG);
    write_u32_at(&file[16], PAGE_UNITS * UNIT_BYTES);
    offset = TEXTURE_FILE_HEADER_SIZE;
    write_u32_at(&file[offset], LITERAL_UNITS);
    offset = offset + 4u;
    for (index = 0; index < LITERAL_UNITS; index++) {
        fill_unit(&file[offset + index * UNIT_BYTES], index);
    }
    offset = offset + LITERAL_UNITS * UNIT_BYTES;
    left = PAGE_UNITS - LITERAL_UNITS;
    while (left > 0) {
        index = left < LITERAL_UNITS ? left : LITERAL_UNITS;
        offset = offset + add_match(&file[offset], index);
        left = left - index;
    }
    fake_files_add(PAGE_PATH, file, offset);
    free(file);
    return offset;
}

static void set_up(void)
{
    fake_memory_reset();
    fake_files_reset();
    read_buffer_forget_reserve();
    CHECK(read_buffer_reserve());
}

static void test_a_packed_page_too_big_for_the_heap_is_read_and_unpacked_in_the_reserved_block(void)
{
    struct finished_read finished;
    unsigned char unit[UNIT_BYTES];
    unsigned int packed_size;

    set_up();
    packed_size = add_packed_page();
    CHECK(packed_size > ONE_MIB);
    fake_memory_set_largest_block(ONE_MIB);
    texture_read_whole(PAGE_PATH, 0, &finished);
    CHECK_INT(finished.result, FILE_OK);
    CHECK_INT(finished.size, TEXTURE_FILE_HEADER_SIZE + PAGE_UNITS * UNIT_BYTES);
    CHECK(read_buffer_holds(finished.data));
    CHECK(!read_buffer_reserve_is_free());
    CHECK(!texture_file_is_packed(finished.data, finished.size));
    fill_unit(unit, 7u);
    CHECK(memcmp(&finished.data[TEXTURE_FILE_HEADER_SIZE + 7u * UNIT_BYTES], unit, UNIT_BYTES) == 0);
    fill_unit(unit, 5u);
    CHECK(memcmp(&finished.data[TEXTURE_FILE_HEADER_SIZE + (LITERAL_UNITS + 5u) * UNIT_BYTES], unit, UNIT_BYTES) == 0);
    read_buffer_release(finished.data);
    CHECK(read_buffer_reserve_is_free());
    CHECK_INT(fake_memory_blocks_in_use(), 1);
    read_buffer_forget_reserve();
}

static void test_a_read_waits_its_turn_when_the_reserved_block_is_held(void)
{
    struct finished_read finished;
    unsigned char *held;

    set_up();
    (void)add_packed_page();
    fake_memory_set_largest_block(ONE_MIB);
    held = read_buffer_take_whole_reserve();
    CHECK(held != NULL);
    texture_read_whole(PAGE_PATH, 0, &finished);
    CHECK_INT(finished.result, FILE_ERR_MEMORY);
    CHECK(finished.data == NULL);
    CHECK_INT(fake_memory_blocks_in_use(), 1);
    read_buffer_release(held);
    read_buffer_forget_reserve();
}

static void test_a_plain_file_too_big_for_the_heap_is_read_into_the_reserved_block(void)
{
    struct finished_read finished;
    unsigned char *file;
    unsigned int size;

    set_up();
    size = 2u * ONE_MIB;
    file = calloc(size, 1);
    memcpy(file, "GTEX", 4);
    file[size - 1u] = 99;
    fake_files_add(PAGE_PATH, file, size);
    free(file);
    fake_memory_set_largest_block(ONE_MIB);
    texture_read_whole(PAGE_PATH, 0, &finished);
    CHECK_INT(finished.result, FILE_OK);
    CHECK_INT(finished.size, size);
    CHECK(read_buffer_holds(finished.data));
    CHECK_INT(finished.data[size - 1u], 99);
    read_buffer_release(finished.data);
    CHECK(read_buffer_reserve_is_free());
    CHECK_INT(fake_memory_blocks_in_use(), 1);
    read_buffer_forget_reserve();
}

static void test_a_packed_file_the_heap_can_hold_is_freed_once_unpacked(void)
{
    struct finished_read finished;

    set_up();
    (void)add_packed_page();
    fake_memory_set_largest_block(8u * ONE_MIB);
    texture_read_whole(PAGE_PATH, 0, &finished);
    CHECK_INT(finished.result, FILE_OK);
    CHECK_INT(fake_memory_blocks_in_use(), 1);
    read_buffer_release(finished.data);
    CHECK(read_buffer_reserve_is_free());
    CHECK_INT(fake_memory_blocks_in_use(), 1);
    read_buffer_forget_reserve();
}

int main(void)
{
    test_a_packed_page_too_big_for_the_heap_is_read_and_unpacked_in_the_reserved_block();
    test_a_read_waits_its_turn_when_the_reserved_block_is_held();
    test_a_plain_file_too_big_for_the_heap_is_read_into_the_reserved_block();
    test_a_packed_file_the_heap_can_hold_is_freed_once_unpacked();
    return test_finish("unit/texture_read");
}
