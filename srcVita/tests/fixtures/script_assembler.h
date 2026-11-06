#ifndef SCRIPT_ASSEMBLER_H
#define SCRIPT_ASSEMBLER_H

#include <stdlib.h>
#include <string.h>

#include "script_file.h"

#define SCRIPT_ASSEMBLER_MAX_WORDS 4096
#define SCRIPT_ASSEMBLER_MAX_STRING_BYTES 4096
#define SCRIPT_ASSEMBLER_MAX_LABEL_BYTES 64
#define SCRIPT_ASSEMBLER_MAX_FILE_BYTES (SCRIPT_FILE_HEADER_SIZE + SCRIPT_ASSEMBLER_MAX_WORDS * SCRIPT_FILE_BYTES_PER_WORD + SCRIPT_ASSEMBLER_MAX_STRING_BYTES + SCRIPT_ASSEMBLER_MAX_LABEL_BYTES)

struct script_assembler {
    unsigned char code[SCRIPT_ASSEMBLER_MAX_WORDS * SCRIPT_FILE_BYTES_PER_WORD];
    unsigned int word_count;
    char strings[SCRIPT_ASSEMBLER_MAX_STRING_BYTES];
    unsigned int string_size;
    unsigned int label_size;
    unsigned char file[SCRIPT_ASSEMBLER_MAX_FILE_BYTES];
    unsigned int file_size;
};

static inline void script_assembler_begin(struct script_assembler *assembler)
{
    memset(assembler, 0, sizeof(*assembler));
}

static inline void script_assembler_word(struct script_assembler *assembler, unsigned int word)
{
    assembler->code[assembler->word_count * SCRIPT_FILE_BYTES_PER_WORD] = (unsigned char)word;
    assembler->code[assembler->word_count * SCRIPT_FILE_BYTES_PER_WORD + 1] = (unsigned char)(word >> 8);
    assembler->word_count = assembler->word_count + 1;
}

static inline void script_assembler_u32(struct script_assembler *assembler, unsigned int value)
{
    script_assembler_word(assembler, value & 0xFFFF);
    script_assembler_word(assembler, value >> 16);
}

static inline unsigned int script_assembler_string(struct script_assembler *assembler, const char *text)
{
    unsigned int offset;

    offset = assembler->string_size;
    strcpy(&assembler->strings[offset], text);
    assembler->string_size = offset + (unsigned int)strlen(text) + 1;
    return offset;
}

static inline void script_assembler_write_u32(unsigned char *bytes, unsigned int value)
{
    bytes[0] = (unsigned char)value;
    bytes[1] = (unsigned char)(value >> 8);
    bytes[2] = (unsigned char)(value >> 16);
    bytes[3] = (unsigned char)(value >> 24);
}

static inline void script_assembler_finish(struct script_assembler *assembler)
{
    unsigned int code_size;
    unsigned char *code_start;

    code_size = assembler->word_count * SCRIPT_FILE_BYTES_PER_WORD;
    code_start = &assembler->file[SCRIPT_FILE_HEADER_SIZE];
    memcpy(assembler->file, "1LMG", 4);
    script_assembler_write_u32(&assembler->file[0x08], code_size);
    script_assembler_write_u32(&assembler->file[0x0C], assembler->string_size);
    script_assembler_write_u32(&assembler->file[0x10], assembler->label_size);
    memcpy(code_start, assembler->code, code_size);
    memcpy(&code_start[code_size], assembler->strings, assembler->string_size);
    assembler->file_size = SCRIPT_FILE_HEADER_SIZE + code_size + assembler->string_size + assembler->label_size;
}

static inline unsigned char *script_assembler_exact_copy(const unsigned char *data, unsigned int size)
{
    unsigned char *copy;

    copy = malloc(size == 0 ? 1 : size);
    memcpy(copy, data, size);
    return copy;
}

#endif
