#include <stdlib.h>

#include "fixtures/script_assembler.h"
#include "script_file.h"
#include "test_support.h"

#define FIRST_WORD 0x1234
#define SECOND_WORD 0x5678
#define THIRD_WORD 0x9ABC
#define HALF_MAX_UNSIGNED 0x7FFFFFFFU
#define NEARLY_MAX_EVEN 0xFFFFFFFEU
#define CODE_SIZE_FIELD 0x08
#define STRING_SIZE_FIELD 0x0C
#define LABEL_SIZE_FIELD 0x10
#define LABEL_BYTES 8
#define LARGE_CODE_BYTES 0x012344U
#define LARGE_STRING_BYTES 0x0456U
#define LARGE_LABEL_BYTES 0x0200U

static struct script_assembler assembler;

static void assemble_three_words_and_two_strings(void)
{
    script_assembler_begin(&assembler);
    script_assembler_word(&assembler, FIRST_WORD);
    script_assembler_word(&assembler, SECOND_WORD);
    script_assembler_word(&assembler, THIRD_WORD);
    script_assembler_string(&assembler, "*");
    script_assembler_string(&assembler, "made_up_name");
    assembler.label_size = LABEL_BYTES;
    script_assembler_finish(&assembler);
}

static int open_exact_copy(const unsigned char *data, unsigned int size)
{
    struct script_file file;
    unsigned char *copy;
    int result;

    copy = script_assembler_exact_copy(data, size);
    result = script_file_open(&file, copy, size);
    free(copy);
    return result;
}

static unsigned char *file_with_sizes(unsigned int code_size, unsigned int string_size, unsigned int label_size, unsigned int file_size)
{
    unsigned char *file_bytes;

    file_bytes = calloc(1, file_size);
    memcpy(file_bytes, "1LMG", 4);
    script_assembler_write_u32(&file_bytes[CODE_SIZE_FIELD], code_size);
    script_assembler_write_u32(&file_bytes[STRING_SIZE_FIELD], string_size);
    script_assembler_write_u32(&file_bytes[LABEL_SIZE_FIELD], label_size);
    return file_bytes;
}

static int open_with_sizes(unsigned int code_size, unsigned int string_size, unsigned int label_size, unsigned int file_size)
{
    unsigned char *file_bytes;
    struct script_file file;
    int result;

    file_bytes = file_with_sizes(code_size, string_size, label_size, file_size);
    result = script_file_open(&file, file_bytes, file_size);
    free(file_bytes);
    return result;
}

static void test_a_good_file_opens(void)
{
    struct script_file file;

    assemble_three_words_and_two_strings();
    CHECK_INT(open_exact_copy(assembler.file, assembler.file_size), SCRIPT_FILE_OK);
    CHECK_INT(script_file_open(&file, assembler.file, assembler.file_size), SCRIPT_FILE_OK);
    CHECK_INT(file.word_count, 3);
    CHECK_INT(file.string_size, 15);
}

static void test_truncated_header_refused(void)
{
    unsigned int size;

    assemble_three_words_and_two_strings();
    for (size = 0; size < SCRIPT_FILE_HEADER_SIZE; size++) {
        CHECK_INT(open_exact_copy(assembler.file, size), SCRIPT_FILE_ERR_TOO_SHORT);
    }
}

static void test_every_cut_of_a_good_file_is_refused(void)
{
    unsigned int size;

    assemble_three_words_and_two_strings();
    for (size = SCRIPT_FILE_HEADER_SIZE; size < assembler.file_size; size++) {
        CHECK_INT(open_exact_copy(assembler.file, size), SCRIPT_FILE_ERR_BAD_SIZES);
    }
}

static void test_wrong_magic_refused(void)
{
    unsigned int index;
    unsigned char bad[SCRIPT_ASSEMBLER_MAX_FILE_BYTES];

    assemble_three_words_and_two_strings();
    for (index = 0; index < 4; index++) {
        memcpy(bad, assembler.file, assembler.file_size);
        bad[index] = (unsigned char)(bad[index] ^ 0x40);
        CHECK_INT(open_exact_copy(bad, assembler.file_size), SCRIPT_FILE_ERR_BAD_MAGIC);
    }
}

static void test_sizes_that_do_not_add_up_refused(void)
{
    const unsigned int file_size = SCRIPT_FILE_HEADER_SIZE + 10;

    CHECK_INT(open_with_sizes(4, 4, 2, file_size), SCRIPT_FILE_OK);
    CHECK_INT(open_with_sizes(6, 4, 2, file_size), SCRIPT_FILE_ERR_BAD_SIZES);
    CHECK_INT(open_with_sizes(4, 5, 2, file_size), SCRIPT_FILE_ERR_BAD_SIZES);
    CHECK_INT(open_with_sizes(4, 4, 3, file_size), SCRIPT_FILE_ERR_BAD_SIZES);
    CHECK_INT(open_with_sizes(4, 4, 1, file_size), SCRIPT_FILE_ERR_BAD_SIZES);
    CHECK_INT(open_with_sizes(NEARLY_MAX_EVEN, 12, 0, file_size), SCRIPT_FILE_ERR_BAD_SIZES);
    CHECK_INT(open_with_sizes(2, 0xFFFFFFF9U, 15, file_size), SCRIPT_FILE_ERR_BAD_SIZES);
    CHECK_INT(open_with_sizes(HALF_MAX_UNSIGNED - 1, HALF_MAX_UNSIGNED, 13, file_size), SCRIPT_FILE_ERR_BAD_SIZES);
    CHECK_INT(open_with_sizes(2, 9, 0xFFFFFFFFU, file_size), SCRIPT_FILE_ERR_BAD_SIZES);
}

static void test_each_byte_of_a_size_field_counts(void)
{
    const unsigned int file_size = SCRIPT_FILE_HEADER_SIZE + 10;

    CHECK_INT(open_with_sizes(0x0104, 4, 2, file_size), SCRIPT_FILE_ERR_BAD_SIZES);
    CHECK_INT(open_with_sizes(0x010004, 4, 2, file_size), SCRIPT_FILE_ERR_BAD_SIZES);
    CHECK_INT(open_with_sizes(0x01000004, 4, 2, file_size), SCRIPT_FILE_ERR_BAD_SIZES);
}

static void test_a_file_with_sizes_past_one_byte_opens(void)
{
    const unsigned int code_size = LARGE_CODE_BYTES;
    const unsigned int string_size = LARGE_STRING_BYTES;
    const unsigned int label_size = LARGE_LABEL_BYTES;
    const unsigned int file_size = SCRIPT_FILE_HEADER_SIZE + code_size + string_size + label_size;
    unsigned char *file_bytes;
    struct script_file file;

    file_bytes = file_with_sizes(code_size, string_size, label_size, file_size);
    CHECK_INT(script_file_open(&file, file_bytes, file_size), SCRIPT_FILE_OK);
    CHECK_INT(file.word_count, code_size / SCRIPT_FILE_BYTES_PER_WORD);
    CHECK_INT(file.string_size, string_size);
    free(file_bytes);
}

static void test_odd_code_size_refused(void)
{
    const unsigned int file_size = SCRIPT_FILE_HEADER_SIZE + 10;

    CHECK_INT(open_with_sizes(3, 5, 2, file_size), SCRIPT_FILE_ERR_ODD_CODE);
}

static void test_word_past_code_is_refused(void)
{
    struct script_file file;
    unsigned int value;

    assemble_three_words_and_two_strings();
    script_file_open(&file, assembler.file, assembler.file_size);
    CHECK_INT(script_file_read_word(&file, 2, &value), SCRIPT_FILE_OK);
    CHECK_INT(value, THIRD_WORD);
    value = 99;
    CHECK_INT(script_file_read_word(&file, 3, &value), SCRIPT_FILE_ERR_OUTSIDE);
    CHECK_INT(script_file_read_word(&file, 0xFFFFFFFFU, &value), SCRIPT_FILE_ERR_OUTSIDE);
    CHECK_INT(value, 99);
}

static void test_word_read_low_byte_first(void)
{
    struct script_file file;
    unsigned int value;

    assemble_three_words_and_two_strings();
    script_file_open(&file, assembler.file, assembler.file_size);
    CHECK_INT(script_file_read_word(&file, 0, &value), SCRIPT_FILE_OK);
    CHECK_INT(value, FIRST_WORD);
    CHECK_INT(assembler.file[SCRIPT_FILE_HEADER_SIZE], 0x34);
    CHECK_INT(assembler.file[SCRIPT_FILE_HEADER_SIZE + 1], 0x12);
}

static void test_u32_read_low_word_first(void)
{
    struct script_file file;
    unsigned int value;

    assemble_three_words_and_two_strings();
    script_file_open(&file, assembler.file, assembler.file_size);
    CHECK_INT(script_file_read_u32(&file, 0, &value), SCRIPT_FILE_OK);
    CHECK_INT(value, (SECOND_WORD << 16) | FIRST_WORD);
    CHECK_INT(script_file_read_u32(&file, 1, &value), SCRIPT_FILE_OK);
    CHECK_INT(value, ((unsigned int)THIRD_WORD << 16) | SECOND_WORD);
}

static void test_u32_past_code_is_refused(void)
{
    struct script_file file;
    unsigned int value;

    assemble_three_words_and_two_strings();
    script_file_open(&file, assembler.file, assembler.file_size);
    value = 99;
    CHECK_INT(script_file_read_u32(&file, 2, &value), SCRIPT_FILE_ERR_OUTSIDE);
    CHECK_INT(script_file_read_u32(&file, 3, &value), SCRIPT_FILE_ERR_OUTSIDE);
    CHECK_INT(script_file_read_u32(&file, 0xFFFFFFFFU, &value), SCRIPT_FILE_ERR_OUTSIDE);
    CHECK_INT(value, 99);
}

static void test_file_offset_maps_to_its_code_word(void)
{
    struct script_file file;
    unsigned int word_index;

    assemble_three_words_and_two_strings();
    script_file_open(&file, assembler.file, assembler.file_size);
    CHECK_INT(script_file_word_of_offset(&file, SCRIPT_FILE_HEADER_SIZE, &word_index), SCRIPT_FILE_OK);
    CHECK_INT(word_index, 0);
    CHECK_INT(script_file_word_of_offset(&file, SCRIPT_FILE_HEADER_SIZE + 4, &word_index), SCRIPT_FILE_OK);
    CHECK_INT(word_index, 2);
    CHECK_INT(script_file_word_of_offset(&file, SCRIPT_FILE_HEADER_SIZE + 6, &word_index), SCRIPT_FILE_OK);
    CHECK_INT(word_index, 3);
}

static void test_file_offset_outside_the_code_is_refused(void)
{
    struct script_file file;
    unsigned int word_index;

    assemble_three_words_and_two_strings();
    script_file_open(&file, assembler.file, assembler.file_size);
    word_index = 99;
    CHECK_INT(script_file_word_of_offset(&file, 0, &word_index), SCRIPT_FILE_ERR_OUTSIDE);
    CHECK_INT(script_file_word_of_offset(&file, SCRIPT_FILE_HEADER_SIZE - 2, &word_index), SCRIPT_FILE_ERR_OUTSIDE);
    CHECK_INT(script_file_word_of_offset(&file, SCRIPT_FILE_HEADER_SIZE + 1, &word_index), SCRIPT_FILE_ERR_OUTSIDE);
    CHECK_INT(script_file_word_of_offset(&file, SCRIPT_FILE_HEADER_SIZE + 8, &word_index), SCRIPT_FILE_ERR_OUTSIDE);
    CHECK_INT(script_file_word_of_offset(&file, 0xFFFFFFFEU, &word_index), SCRIPT_FILE_ERR_OUTSIDE);
    CHECK_INT(word_index, 99);
}

static void test_file_offset_below_the_code_is_refused_whatever_the_code_size(void)
{
    struct script_file file;
    unsigned int word_index;

    memset(&file, 0, sizeof(file));
    file.word_count = 0xFFFFFFFFU;
    word_index = 99;
    CHECK_INT(script_file_word_of_offset(&file, 0, &word_index), SCRIPT_FILE_ERR_OUTSIDE);
    CHECK_INT(script_file_word_of_offset(&file, SCRIPT_FILE_HEADER_SIZE - 2, &word_index), SCRIPT_FILE_ERR_OUTSIDE);
    CHECK_INT(word_index, 99);
}

static void test_string_reads_from_the_string_table(void)
{
    struct script_file file;
    unsigned char *copy;
    const char *text;

    assemble_three_words_and_two_strings();
    copy = script_assembler_exact_copy(assembler.file, assembler.file_size);
    CHECK_INT(script_file_open(&file, copy, assembler.file_size), SCRIPT_FILE_OK);
    text = script_file_string(&file, 0);
    CHECK(text != NULL);
    CHECK_TEXT(text, "*");
    text = script_file_string(&file, 2);
    CHECK(text != NULL);
    CHECK_TEXT(text, "made_up_name");
    free(copy);
}

static void test_string_past_table_is_refused(void)
{
    struct script_file file;
    unsigned char *copy;

    assemble_three_words_and_two_strings();
    copy = script_assembler_exact_copy(assembler.file, assembler.file_size);
    script_file_open(&file, copy, assembler.file_size);
    CHECK(script_file_string(&file, file.string_size) == NULL);
    CHECK(script_file_string(&file, file.string_size + LABEL_BYTES) == NULL);
    CHECK(script_file_string(&file, 0xFFFFFFFFU) == NULL);
    free(copy);
}

static void test_string_with_no_end_is_refused(void)
{
    struct script_file file;
    unsigned char *copy;

    script_assembler_begin(&assembler);
    script_assembler_word(&assembler, FIRST_WORD);
    memcpy(assembler.strings, "no_end", 6);
    assembler.string_size = 6;
    script_assembler_finish(&assembler);
    copy = script_assembler_exact_copy(assembler.file, assembler.file_size);
    CHECK_INT(script_file_open(&file, copy, assembler.file_size), SCRIPT_FILE_OK);
    CHECK(script_file_string(&file, 0) == NULL);
    CHECK(script_file_string(&file, 5) == NULL);
    free(copy);
}

static void test_a_file_with_no_code_or_strings_reads_nothing(void)
{
    struct script_file file;
    unsigned char *copy;
    unsigned int value;

    script_assembler_begin(&assembler);
    script_assembler_finish(&assembler);
    copy = script_assembler_exact_copy(assembler.file, assembler.file_size);
    CHECK_INT(script_file_open(&file, copy, assembler.file_size), SCRIPT_FILE_OK);
    CHECK_INT(script_file_read_word(&file, 0, &value), SCRIPT_FILE_ERR_OUTSIDE);
    CHECK_INT(script_file_read_u32(&file, 0, &value), SCRIPT_FILE_ERR_OUTSIDE);
    CHECK(script_file_string(&file, 0) == NULL);
    free(copy);
}

int main(void)
{
    test_a_good_file_opens();
    test_truncated_header_refused();
    test_every_cut_of_a_good_file_is_refused();
    test_wrong_magic_refused();
    test_sizes_that_do_not_add_up_refused();
    test_each_byte_of_a_size_field_counts();
    test_a_file_with_sizes_past_one_byte_opens();
    test_odd_code_size_refused();
    test_word_past_code_is_refused();
    test_word_read_low_byte_first();
    test_u32_read_low_word_first();
    test_u32_past_code_is_refused();
    test_file_offset_maps_to_its_code_word();
    test_file_offset_outside_the_code_is_refused();
    test_file_offset_below_the_code_is_refused_whatever_the_code_size();
    test_string_reads_from_the_string_table();
    test_string_past_table_is_refused();
    test_string_with_no_end_is_refused();
    test_a_file_with_no_code_or_strings_reads_nothing();
    return test_finish("unit/script_file");
}
