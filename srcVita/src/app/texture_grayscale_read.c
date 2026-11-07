#include <stddef.h>
#include "background_reader.h"
#include "texture_file.h"

void background_reader_make_grayscale(struct finished_read *finished)
{
    struct texture_file_header header;

    if (finished->data == NULL || texture_file_read_header(finished->data, finished->size, &header) != TEXTURE_FILE_OK) {
        return;
    }
    texture_file_make_grayscale(&header, &finished->data[TEXTURE_FILE_HEADER_SIZE]);
}
