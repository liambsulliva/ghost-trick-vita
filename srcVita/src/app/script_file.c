#include "script_file.h"

#include <string.h>

#define MAGIC_SIZE 4
#define CODE_SIZE_OFFSET 0x08
#define STRING_SIZE_OFFSET 0x0C
#define LABEL_SIZE_OFFSET 0x10
#define WORDS_IN_U32 2

static const unsigned char MAGIC[MAGIC_SIZE] = {'1', 'L', 'M', 'G'};

static unsigned int read_u32_little_endian(const unsigned char *bytes)
{
    unsigned int value;

    value = bytes[0];
    value = value | ((unsigned int)bytes[1] << 8);
    value = value | ((unsigned int)bytes[2] << 16);
    value = value | ((unsigned int)bytes[3] << 24);
    return value;
}

static int sizes_add_up(unsigned int size, unsigned int code_size, unsigned int string_size, unsigned int label_size)
{
    unsigned int remaining;

    remaining = size - SCRIPT_FILE_HEADER_SIZE;
    if (code_size > remaining) {
        return 0;
    }
    remaining = remaining - code_size;
    if (string_size > remaining) {
        return 0;
    }
    remaining = remaining - string_size;
    return label_size == remaining;
}

int script_file_open(struct script_file *file, unsigned char *data, unsigned int size)
{
    unsigned int code_size;
    unsigned int string_size;
    unsigned int label_size;

    if (size < SCRIPT_FILE_HEADER_SIZE) {
        return SCRIPT_FILE_ERR_TOO_SHORT;
    }
    if (memcmp(data, MAGIC, MAGIC_SIZE) != 0) {
        return SCRIPT_FILE_ERR_BAD_MAGIC;
    }
    code_size = read_u32_little_endian(&data[CODE_SIZE_OFFSET]);
    string_size = read_u32_little_endian(&data[STRING_SIZE_OFFSET]);
    label_size = read_u32_little_endian(&data[LABEL_SIZE_OFFSET]);
    if (!sizes_add_up(size, code_size, string_size, label_size)) {
        return SCRIPT_FILE_ERR_BAD_SIZES;
    }
    if (code_size % SCRIPT_FILE_BYTES_PER_WORD != 0) {
        return SCRIPT_FILE_ERR_ODD_CODE;
    }
    file->code = &data[SCRIPT_FILE_HEADER_SIZE];
    file->word_count = code_size / SCRIPT_FILE_BYTES_PER_WORD;
    file->strings = &data[SCRIPT_FILE_HEADER_SIZE + code_size];
    file->string_size = string_size;
    return SCRIPT_FILE_OK;
}

static unsigned int word_at(const struct script_file *file, unsigned int word_index)
{
    const unsigned char *bytes;

    bytes = &file->code[word_index * SCRIPT_FILE_BYTES_PER_WORD];
    return bytes[0] | ((unsigned int)bytes[1] << 8);
}

int script_file_read_word(const struct script_file *file, unsigned int word_index, unsigned int *value)
{
    if (word_index >= file->word_count) {
        return SCRIPT_FILE_ERR_OUTSIDE;
    }
    *value = word_at(file, word_index);
    return SCRIPT_FILE_OK;
}

int script_file_write_word(struct script_file *file, unsigned int word_index, unsigned int value)
{
    unsigned char *bytes;

    if (word_index >= file->word_count) {
        return SCRIPT_FILE_ERR_OUTSIDE;
    }
    bytes = &file->code[word_index * SCRIPT_FILE_BYTES_PER_WORD];
    bytes[0] = (unsigned char)(value & 0xff);
    bytes[1] = (unsigned char)((value >> 8) & 0xff);
    return SCRIPT_FILE_OK;
}

int script_file_read_u32(const struct script_file *file, unsigned int word_index, unsigned int *value)
{
    if (word_index >= file->word_count || file->word_count - word_index < WORDS_IN_U32) {
        return SCRIPT_FILE_ERR_OUTSIDE;
    }
    *value = word_at(file, word_index) | (word_at(file, word_index + 1) << 16);
    return SCRIPT_FILE_OK;
}

int script_file_word_of_offset(const struct script_file *file, unsigned int file_offset, unsigned int *word_index)
{
    unsigned int code_offset;

    if (file_offset < SCRIPT_FILE_HEADER_SIZE) {
        return SCRIPT_FILE_ERR_OUTSIDE;
    }
    code_offset = file_offset - SCRIPT_FILE_HEADER_SIZE;
    if (code_offset % SCRIPT_FILE_BYTES_PER_WORD != 0) {
        return SCRIPT_FILE_ERR_OUTSIDE;
    }
    if (code_offset / SCRIPT_FILE_BYTES_PER_WORD > file->word_count) {
        return SCRIPT_FILE_ERR_OUTSIDE;
    }
    *word_index = code_offset / SCRIPT_FILE_BYTES_PER_WORD;
    return SCRIPT_FILE_OK;
}

const char *script_file_string(const struct script_file *file, unsigned int string_offset)
{
    if (string_offset >= file->string_size) {
        return NULL;
    }
    if (memchr(&file->strings[string_offset], '\0', file->string_size - string_offset) == NULL) {
        return NULL;
    }
    return (const char *)&file->strings[string_offset];
}
