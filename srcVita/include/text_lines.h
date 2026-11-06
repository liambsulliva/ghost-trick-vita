#ifndef TEXT_LINES_H
#define TEXT_LINES_H

unsigned int text_lines_read_next(const char *text, unsigned int text_size, unsigned int offset, char *line, unsigned int line_size);
int text_line_starts_with(const char *line, const char *word);

#endif
