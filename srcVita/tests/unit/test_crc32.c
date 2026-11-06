#include "crc32.h"
#include "test_support.h"

#define CHECK_VALUE 0xCBF43926U

static void test_the_standard_check_value(void)
{
    const char text[] = "123456789";

    CHECK(crc32_add(0, (const unsigned char *)text, 9) == CHECK_VALUE);
}

static void test_split_calls_match_one_call(void)
{
    const char text[] = "123456789";
    unsigned int crc;

    crc = crc32_add(0, (const unsigned char *)text, 4);
    crc = crc32_add(crc, (const unsigned char *)&text[4], 5);
    CHECK(crc == CHECK_VALUE);
}

static void test_no_bytes_leave_the_value_alone(void)
{
    CHECK(crc32_add(0, (const unsigned char *)"", 0) == 0);
    CHECK(crc32_add(CHECK_VALUE, (const unsigned char *)"", 0) == CHECK_VALUE);
}

int main(void)
{
    test_the_standard_check_value();
    test_split_calls_match_one_call();
    test_no_bytes_leave_the_value_alone();
    return test_finish("unit/crc32");
}
