#include <string.h>
#include "script_pack.h"
#include "test_support.h"
#include "fixtures/script_pack_bytes.h"

static unsigned int make_pack(unsigned char *bytes, const char *first, const char *first_data, const char *second, const char *second_data)
{
    const char *names[2];
    const char *datas[2];

    names[0] = first;
    names[1] = second;
    datas[0] = first_data;
    datas[1] = second_data;
    return make_script_pack(bytes, names, datas, 2);
}

static void test_a_file_is_found_by_name_and_extension(void)
{
    unsigned char bytes[256];
    struct script_pack pack;
    const unsigned char *data;
    unsigned int size;
    unsigned int pack_size;

    pack_size = make_pack(bytes, "chain.txt", "file a\n", "a.lmg", "script");
    CHECK(script_pack_open(&pack, bytes, pack_size));
    CHECK_INT((int)pack.count, 2);
    CHECK(script_pack_find(&pack, "a", ".lmg", &data, &size));
    CHECK_INT((int)size, 6);
    CHECK(memcmp(data, "script", 6) == 0);
    CHECK(script_pack_find(&pack, "chain", ".txt", &data, &size));
    CHECK(!script_pack_find(&pack, "a", ".cues", &data, &size));
    CHECK(!script_pack_find(&pack, "a.lm", "", &data, &size));
}

static void test_a_cut_or_foreign_pack_is_refused(void)
{
    unsigned char bytes[256];
    struct script_pack pack;
    unsigned int pack_size;

    pack_size = make_pack(bytes, "chain.txt", "file a\n", "a.lmg", "script");
    CHECK(!script_pack_open(&pack, bytes, pack_size - 1));
    CHECK(!script_pack_open(&pack, bytes, 20));
    bytes[0] = 'X';
    CHECK(!script_pack_open(&pack, bytes, pack_size));
    bytes[0] = 'G';
    bytes[4] = 2;
    CHECK(!script_pack_open(&pack, bytes, pack_size));
}

int main(void)
{
    test_a_file_is_found_by_name_and_extension();
    test_a_cut_or_foreign_pack_is_refused();
    return test_finish("unit/script_pack");
}
