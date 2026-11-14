#include "fakes/fake_platform.h"
#include "memory.h"
#include "read_buffer.h"
#include "test_support.h"

#define SIXTEEN_MIB (16u * 1024u * 1024u)

static void test_a_large_unpack_gets_the_reserved_block_when_the_heap_has_no_room(void)
{
    unsigned char *first;
    unsigned char *second;

    fake_memory_reset();
    read_buffer_forget_reserve();
    CHECK(read_buffer_reserve());
    fake_memory_set_largest_block(8u * 1024u * 1024u);
    first = read_buffer_take(SIXTEEN_MIB + 20u);
    CHECK(first != NULL);
    CHECK(!read_buffer_reserve_is_free());
    second = read_buffer_take(SIXTEEN_MIB + 20u);
    CHECK(second == NULL);
    read_buffer_release(first);
    CHECK(read_buffer_reserve_is_free());
    second = read_buffer_take(SIXTEEN_MIB + 20u);
    CHECK(second == first);
    read_buffer_release(second);
    read_buffer_forget_reserve();
}

static void test_small_reads_never_hold_the_reserved_block(void)
{
    unsigned char *small;

    fake_memory_reset();
    read_buffer_forget_reserve();
    CHECK(read_buffer_reserve());
    small = read_buffer_take(4u * 1024u * 1024u);
    CHECK(small != NULL);
    CHECK(read_buffer_reserve_is_free());
    read_buffer_release(small);
    CHECK_INT(fake_memory_blocks_in_use(), 1);
    read_buffer_forget_reserve();
    CHECK_INT(fake_memory_blocks_in_use(), 0);
}

static void test_a_second_large_read_uses_the_heap_while_the_block_is_held(void)
{
    unsigned char *first;
    unsigned char *second;

    fake_memory_reset();
    read_buffer_forget_reserve();
    CHECK(read_buffer_reserve());
    first = read_buffer_take(SIXTEEN_MIB);
    second = read_buffer_take(SIXTEEN_MIB);
    CHECK(first != NULL);
    CHECK(second != NULL);
    CHECK(second != first);
    read_buffer_release(second);
    read_buffer_release(first);
    CHECK(read_buffer_reserve_is_free());
    read_buffer_forget_reserve();
}

int main(void)
{
    test_a_large_unpack_gets_the_reserved_block_when_the_heap_has_no_room();
    test_small_reads_never_hold_the_reserved_block();
    test_a_second_large_read_uses_the_heap_while_the_block_is_held();
    return test_finish("unit/read_buffer");
}
