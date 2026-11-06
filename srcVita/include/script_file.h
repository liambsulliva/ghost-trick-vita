#ifndef SCRIPT_FILE_H
#define SCRIPT_FILE_H

#define SCRIPT_FILE_HEADER_SIZE 0x34
#define SCRIPT_FILE_BYTES_PER_WORD 2

#define SCRIPT_FILE_OK 0
#define SCRIPT_FILE_ERR_TOO_SHORT -1
#define SCRIPT_FILE_ERR_BAD_MAGIC -2
#define SCRIPT_FILE_ERR_BAD_SIZES -3
#define SCRIPT_FILE_ERR_ODD_CODE -4
#define SCRIPT_FILE_ERR_OUTSIDE -5

struct script_file {
    const unsigned char *code;
    unsigned int word_count;
    const unsigned char *strings;
    unsigned int string_size;
};

int script_file_open(struct script_file *file, const unsigned char *data, unsigned int size);
int script_file_read_word(const struct script_file *file, unsigned int word_index, unsigned int *value);
int script_file_read_u32(const struct script_file *file, unsigned int word_index, unsigned int *value);
int script_file_word_of_offset(const struct script_file *file, unsigned int file_offset, unsigned int *word_index);
const char *script_file_string(const struct script_file *file, unsigned int string_offset);

#endif
