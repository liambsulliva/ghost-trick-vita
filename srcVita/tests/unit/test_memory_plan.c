#include "memory.h"
#include "read_buffer.h"
#include "test_support.h"

static void test_the_heap_grows_by_the_reserved_read_block_and_keeps_its_general_room(void)
{
    CHECK(MEMORY_HEAP_BYTES >= MEMORY_GENERAL_HEAP_BYTES + READ_BUFFER_RESERVED_BYTES - 64u);
    CHECK(MEMORY_GENERAL_HEAP_BYTES >= 112u * 1024u * 1024u);
}

int main(void)
{
    test_the_heap_grows_by_the_reserved_read_block_and_keeps_its_general_room();
    return test_finish("unit/memory_plan");
}
