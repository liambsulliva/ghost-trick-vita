#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "animation.h"
#include "packed_file.h"
#include "test_support.h"

#define BUNDLE_FOLDER "../extracted/vita_assets/game/scripts"

static FILE *open_unpacked(const char *path)
{
    FILE *packed_file;
    unsigned char *packed;
    unsigned char *plain;
    long length;
    unsigned int plain_size;

    packed_file = fopen(path, "rb");
    if (packed_file == NULL) {
        return NULL;
    }
    fseek(packed_file, 0, SEEK_END);
    length = ftell(packed_file);
    fseek(packed_file, 0, SEEK_SET);
    packed = malloc((size_t)length);
    if (packed == NULL || fread(packed, 1, (size_t)length, packed_file) != (size_t)length) {
        fclose(packed_file);
        free(packed);
        return NULL;
    }
    fclose(packed_file);
    if (!packed_file_is_packed(packed, (unsigned int)length)) {
        return fmemopen(packed, (size_t)length, "r");
    }
    plain_size = packed_file_unpacked_size(packed);
    plain = malloc(plain_size + 1u);
    if (plain == NULL || !packed_file_unpack(packed, (unsigned int)length, plain, plain_size)) {
        free(packed);
        free(plain);
        return NULL;
    }
    free(packed);
    return fmemopen(plain, plain_size, "r");
}

static double seconds_now(void)
{
    struct timespec now;

    clock_gettime(CLOCK_MONOTONIC, &now);
    return (double)now.tv_sec + (double)now.tv_nsec / 1e9;
}

static void test_the_quick_reader_matches_sscanf_on_every_baked_frame(void)
{
    DIR *folder;
    struct dirent *entry;
    char path[512];
    char line[512];
    long lines;
    long mismatches;
    double quick_seconds;
    double scan_seconds;

    folder = opendir(BUNDLE_FOLDER);
    if (folder == NULL) {
        printf("no baked bundles, skipped\n");
        return;
    }
    lines = 0;
    mismatches = 0;
    quick_seconds = 0.0;
    scan_seconds = 0.0;
    while ((entry = readdir(folder)) != NULL) {
        FILE *file;
        size_t length;

        length = strlen(entry->d_name);
        if (length < 6 || strcmp(&entry->d_name[length - 6], ".anims") != 0) {
            continue;
        }
        snprintf(path, sizeof(path), "%s/%s", BUNDLE_FOLDER, entry->d_name);
        file = open_unpacked(path);
        while (file != NULL && fgets(line, sizeof(line), file) != NULL) {
            struct animation quick;
            struct animation_frame scanned;
            char text[600];
            double started;

            if (strncmp(line, "frame ", 6) != 0) {
                continue;
            }
            line[strcspn(line, "\n")] = '\0';
            snprintf(text, sizeof(text), "animation t\nloops 0\n%s\n", line);
            started = seconds_now();
            if (animation_parse(text, (unsigned int)strlen(text), &quick) != ANIMATION_OK) {
                mismatches = mismatches + 1;
                continue;
            }
            quick_seconds = quick_seconds + seconds_now() - started;
            started = seconds_now();
            sscanf(line, "frame %95s %u %f %f %f %f %f %f %f %f", scanned.texture_name, &scanned.ticks, &scanned.x, &scanned.y, &scanned.width, &scanned.height, &scanned.u, &scanned.v, &scanned.u_size, &scanned.v_size);
            scan_seconds = scan_seconds + seconds_now() - started;
            if (strcmp(quick.frames[0].texture_name, scanned.texture_name) != 0 || quick.frames[0].ticks != scanned.ticks || quick.frames[0].x != scanned.x || quick.frames[0].y != scanned.y || quick.frames[0].width != scanned.width || quick.frames[0].height != scanned.height || quick.frames[0].u != scanned.u || quick.frames[0].v != scanned.v || quick.frames[0].u_size != scanned.u_size || quick.frames[0].v_size != scanned.v_size) {
                if (mismatches < 5) {
                    printf("mismatch: %s\n", line);
                }
                mismatches = mismatches + 1;
            }
            animation_free_frames(&quick);
            lines = lines + 1;
        }
        if (file != NULL) {
            fclose(file);
        }
    }
    closedir(folder);
    printf("frame lines %ld, mismatches %ld, whole parse %.3f s, sscanf alone %.3f s\n", lines, mismatches, quick_seconds, scan_seconds);
    CHECK(lines > 0);
    CHECK_INT((int)mismatches, 0);
}

int main(void)
{
    test_the_quick_reader_matches_sscanf_on_every_baked_frame();
    return test_finish("unit/frame_line_parse");
}
