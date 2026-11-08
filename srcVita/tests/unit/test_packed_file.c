#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crc32.h"
#include "packed_file.h"
#include "test_support.h"

#define MANIFEST "/private/tmp/claude-501/-Users-liambsulliva-Developer-ghost-trick-vita/e3f1daaa-125f-4131-a71b-113a6314608b/scratchpad/fx/vectors/manifest.txt"
#define PATH_LIMIT 1024

static unsigned char *read_whole(const char *path, unsigned int *size)
{
    FILE *file;
    unsigned char *bytes;
    long length;

    file = fopen(path, "rb");
    if (file == NULL) {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    bytes = malloc((size_t)length + 1u);
    if (bytes != NULL && fread(bytes, 1, (size_t)length, file) != (size_t)length) {
        free(bytes);
        bytes = NULL;
    }
    fclose(file);
    *size = (unsigned int)length;
    return bytes;
}

static void test_crc_matches_the_reference(void)
{
    CHECK(crc32_add(0u, (const unsigned char *)"123456789", 9u) == 0xcbf43926u);
}

static void test_every_vector_unpacks_to_its_reference(void)
{
    FILE *manifest;
    char path[PATH_LIMIT];
    unsigned int expected_size;
    unsigned int expected_crc;
    int vectors;
    int matches;

    manifest = fopen(MANIFEST, "r");
    CHECK(manifest != NULL);
    if (manifest == NULL) {
        return;
    }
    vectors = 0;
    matches = 0;
    while (fscanf(manifest, "%1023s %u %x", path, &expected_size, &expected_crc) == 3) {
        unsigned char *packed;
        unsigned char *plain;
        unsigned int size;

        vectors = vectors + 1;
        packed = read_whole(path, &size);
        if (packed == NULL || !packed_file_is_packed(packed, size) || packed_file_unpacked_size(packed) != expected_size) {
            printf("bad header %s\n", path);
            free(packed);
            continue;
        }
        plain = malloc(expected_size + 1u);
        if (plain != NULL && packed_file_unpack(packed, size, plain, expected_size) && crc32_add(0u, plain, expected_size) == expected_crc) {
            matches = matches + 1;
        } else {
            printf("mismatch %s\n", path);
        }
        free(plain);
        free(packed);
    }
    fclose(manifest);
    CHECK(vectors > 100);
    CHECK_INT(matches, vectors);
}

static void test_damaged_streams_are_refused(void)
{
    static const unsigned char header_only[] = { 'G', 'P', 'K', '1', 4, 0, 0, 0 };
    static const unsigned char wrong_magic[] = { 'G', 'P', 'K', '2', 0, 0, 0, 0, 3, 0 };
    unsigned char out[8];

    CHECK(!packed_file_unpack(header_only, sizeof(header_only), out, 4u));
    CHECK(!packed_file_is_packed(wrong_magic, sizeof(wrong_magic)));
    CHECK(!packed_file_inflate((const unsigned char *)"\x07", 1u, out, sizeof(out)));
}

int main(void)
{
    test_crc_matches_the_reference();
    test_every_vector_unpacks_to_its_reference();
    test_damaged_streams_are_refused();
    return test_finish("unit/packed_file");
}
