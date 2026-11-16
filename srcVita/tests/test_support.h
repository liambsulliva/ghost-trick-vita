#ifndef TEST_SUPPORT_H
#define TEST_SUPPORT_H

#include <stdio.h>
#include <string.h>

static int test_failure_count;
static int test_check_count;

#define CHECK(condition) \
    do { \
        test_check_count = test_check_count + 1; \
        if (!(condition)) { \
            printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition); \
            test_failure_count = test_failure_count + 1; \
        } \
    } while (0)

#define CHECK_INT(actual, expected) \
    do { \
        long long actual_value = (long long)(actual); \
        long long expected_value = (long long)(expected); \
        test_check_count = test_check_count + 1; \
        if (actual_value != expected_value) { \
            printf("FAIL %s:%d: %s is %lld, expected %lld\n", __FILE__, __LINE__, #actual, actual_value, expected_value); \
            test_failure_count = test_failure_count + 1; \
        } \
    } while (0)

#define CHECK_TEXT(actual, expected) \
    do { \
        test_check_count = test_check_count + 1; \
        if (strcmp((actual), (expected)) != 0) { \
            printf("FAIL %s:%d: %s is \"%s\", expected \"%s\"\n", __FILE__, __LINE__, #actual, (actual), (expected)); \
            test_failure_count = test_failure_count + 1; \
        } \
    } while (0)

#define CHECK_HAS_TEXT(actual, wanted) \
    do { \
        test_check_count = test_check_count + 1; \
        if (strstr((actual), (wanted)) == NULL) { \
            printf("FAIL %s:%d: \"%s\" does not contain \"%s\"\n", __FILE__, __LINE__, (actual), (wanted)); \
            test_failure_count = test_failure_count + 1; \
        } \
    } while (0)

#define TENTHS 10.0f

static inline void put_u16(unsigned char *at, unsigned int value)
{
    at[0] = (unsigned char)(value & 0xffu);
    at[1] = (unsigned char)(value >> 8);
}

static inline int nearly(float actual, float expected)
{
    float difference;

    difference = actual - expected;
    if (difference < 0.0f) {
        difference = -difference;
    }
    return difference < 0.01f;
}

static inline int tenths(float value)
{
    return (int)(value * TENTHS + (value < 0.0f ? -0.5f : 0.5f));
}

static int test_finish(const char *suite_name)
{
    printf("%s: %d checks, %d failures\n", suite_name, test_check_count, test_failure_count);
    return test_failure_count == 0 ? 0 : 1;
}

#endif
