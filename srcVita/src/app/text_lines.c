#include "text_lines.h"

unsigned int text_lines_read_next(const char *text, unsigned int text_size, unsigned int offset, char *line, unsigned int line_size)
{
    unsigned int length;

    length = 0;
    while (offset < text_size && text[offset] != '\n') {
        if (text[offset] != '\r' && length + 1 < line_size) {
            line[length] = text[offset];
            length = length + 1;
        }
        offset = offset + 1;
    }
    line[length] = '\0';
    if (offset < text_size) {
        offset = offset + 1;
    }
    return offset;
}
