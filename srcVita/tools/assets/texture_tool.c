#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "astc_decode.h"
#include "etc_decode.h"

#define TEX_HEADER_SIZE 40
#define TEX_MIP_RECORD_SIZE 16
#define TEX_MAX_SIDE 16384
#define FORMAT_RGBA8_LINEAR 28
#define FORMAT_RGBA8_SRGB 29
#define FORMAT_ASTC_6X6_LINEAR 1038
#define FORMAT_ASTC_6X6_SRGB 1039
#define FORMAT_ETC2_RGB_LINEAR 1086
#define FORMAT_ETC2_RGB_SRGB 1087

struct tex_image {
    unsigned int width;
    unsigned int height;
    unsigned int format;
    const unsigned char *mip_data;
    size_t mip_size;
};

struct rectangle {
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
};

struct span {
    unsigned int first;
    unsigned int last;
    double first_weight;
    double last_weight;
};

static unsigned int read_u16(const unsigned char *bytes)
{
    return (unsigned int)bytes[0] | ((unsigned int)bytes[1] << 8);
}

static unsigned int read_u32(const unsigned char *bytes)
{
    return read_u16(bytes) | (read_u16(bytes + 2) << 16);
}

static unsigned long long read_u64(const unsigned char *bytes)
{
    return (unsigned long long)read_u32(bytes) | ((unsigned long long)read_u32(bytes + 4) << 32);
}

static unsigned char *read_whole_file(const char *path, size_t *size)
{
    FILE *file;
    long length;
    unsigned char *data;

    file = fopen(path, "rb");
    if (file == NULL) {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (length < 0) {
        fclose(file);
        return NULL;
    }
    data = malloc((size_t)length + 1);
    if (data == NULL) {
        fclose(file);
        return NULL;
    }
    if (fread(data, 1, (size_t)length, file) != (size_t)length) {
        free(data);
        fclose(file);
        return NULL;
    }
    fclose(file);
    *size = (size_t)length;
    return data;
}

static int block_layout(unsigned int format, unsigned int *block_width, unsigned int *block_height, unsigned int *block_bytes)
{
    if (format == FORMAT_ASTC_6X6_LINEAR || format == FORMAT_ASTC_6X6_SRGB) {
        *block_width = 6;
        *block_height = 6;
        *block_bytes = 16;
        return 1;
    }
    if (format == FORMAT_ETC2_RGB_LINEAR || format == FORMAT_ETC2_RGB_SRGB) {
        *block_width = 4;
        *block_height = 4;
        *block_bytes = 8;
        return 1;
    }
    if (format == FORMAT_RGBA8_LINEAR || format == FORMAT_RGBA8_SRGB) {
        *block_width = 1;
        *block_height = 1;
        *block_bytes = 4;
        return 1;
    }
    return 0;
}

static const char *read_tex_header(const unsigned char *data, size_t size, struct tex_image *image)
{
    unsigned int block_width;
    unsigned int block_height;
    unsigned int block_bytes;
    unsigned int mip_count;
    unsigned long long mip_offset;
    unsigned long long expected_size;

    if (size < TEX_HEADER_SIZE + TEX_MIP_RECORD_SIZE) {
        return "file is too short";
    }
    if (memcmp(data, "TEX\0", 4) != 0) {
        return "file is not a TEX file";
    }
    image->width = read_u16(data + 8);
    image->height = read_u16(data + 10);
    image->format = read_u32(data + 16);
    mip_count = data[15] / 16;
    if (image->width == 0 || image->height == 0 || image->width > TEX_MAX_SIDE || image->height > TEX_MAX_SIDE) {
        return "bad image size";
    }
    if (read_u16(data + 12) != 1 || data[14] != 1 || mip_count == 0) {
        return "only plain 2D images are supported";
    }
    if (!block_layout(image->format, &block_width, &block_height, &block_bytes)) {
        return "unsupported format code";
    }
    expected_size = (unsigned long long)((image->width + block_width - 1) / block_width);
    expected_size = expected_size * ((image->height + block_height - 1) / block_height) * block_bytes;
    mip_offset = read_u64(data + TEX_HEADER_SIZE);
    if (read_u32(data + TEX_HEADER_SIZE + 12) != expected_size) {
        return "top mip size does not match the format";
    }
    if (mip_offset > size || expected_size > size - mip_offset) {
        return "top mip runs past the end of the file";
    }
    image->mip_data = data + mip_offset;
    image->mip_size = (size_t)expected_size;
    return NULL;
}

static unsigned char *decode_top_mip(const struct tex_image *image)
{
    unsigned char *pixels;

    pixels = malloc((size_t)image->width * image->height * 4);
    if (pixels == NULL) {
        return NULL;
    }
    if (image->format == FORMAT_ASTC_6X6_LINEAR) {
        astc_decode_image(image->mip_data, (int)image->width, (int)image->height, 6, 6, 0, pixels);
    } else if (image->format == FORMAT_ASTC_6X6_SRGB) {
        astc_decode_image(image->mip_data, (int)image->width, (int)image->height, 6, 6, 1, pixels);
    } else if (image->format == FORMAT_ETC2_RGB_LINEAR || image->format == FORMAT_ETC2_RGB_SRGB) {
        etc2_rgb8_decode_image(image->mip_data, (int)image->width, (int)image->height, pixels, NULL);
    } else {
        memcpy(pixels, image->mip_data, image->mip_size);
    }
    return pixels;
}

static unsigned char *crop_pixels(const unsigned char *pixels, unsigned int width, const struct rectangle *crop)
{
    unsigned char *cropped;
    unsigned int row;

    cropped = malloc((size_t)crop->width * crop->height * 4);
    if (cropped == NULL) {
        return NULL;
    }
    for (row = 0; row < crop->height; row++) {
        size_t source_index = ((size_t)(crop->y + row) * width + crop->x) * 4;
        size_t target_index = (size_t)row * crop->width * 4;
        memcpy(cropped + target_index, pixels + source_index, (size_t)crop->width * 4);
    }
    return cropped;
}

static struct span *make_spans(unsigned int source_size, unsigned int target_size)
{
    struct span *spans;
    unsigned int index;

    spans = malloc(sizeof(struct span) * target_size);
    if (spans == NULL) {
        return NULL;
    }
    for (index = 0; index < target_size; index++) {
        double start = (double)index * source_size / target_size;
        double end = (double)(index + 1) * source_size / target_size;
        unsigned int first = (unsigned int)start;
        unsigned int last = (unsigned int)end;

        if ((double)last == end) {
            last = last - 1;
        }
        spans[index].first = first;
        spans[index].last = last;
        if (first == last) {
            spans[index].first_weight = end - start;
            spans[index].last_weight = end - start;
        } else {
            spans[index].first_weight = (double)(first + 1) - start;
            spans[index].last_weight = end - (double)last;
        }
    }
    return spans;
}

static double span_weight(const struct span *span, unsigned int position)
{
    if (position == span->first) {
        return span->first_weight;
    }
    if (position == span->last) {
        return span->last_weight;
    }
    return 1.0;
}

static unsigned char round_to_byte(double value)
{
    if (value <= 0.0) {
        return 0;
    }
    if (value >= 255.0) {
        return 255;
    }
    return (unsigned char)(value + 0.5);
}

static void average_one_pixel(const unsigned char *source, unsigned int source_width, const struct span *column, const struct span *row, unsigned char *target)
{
    double weight_total = 0.0;
    double alpha_total = 0.0;
    double red_total = 0.0;
    double green_total = 0.0;
    double blue_total = 0.0;
    unsigned int y;
    unsigned int x;

    for (y = row->first; y <= row->last; y++) {
        double row_weight = span_weight(row, y);

        for (x = column->first; x <= column->last; x++) {
            const unsigned char *pixel = source + ((size_t)y * source_width + x) * 4;
            double weight = row_weight * span_weight(column, x);
            double visible_weight = weight * pixel[3];

            weight_total = weight_total + weight;
            alpha_total = alpha_total + visible_weight;
            red_total = red_total + visible_weight * pixel[0];
            green_total = green_total + visible_weight * pixel[1];
            blue_total = blue_total + visible_weight * pixel[2];
        }
    }
    target[3] = round_to_byte(alpha_total / weight_total);
    if (alpha_total > 0.0) {
        target[0] = round_to_byte(red_total / alpha_total);
        target[1] = round_to_byte(green_total / alpha_total);
        target[2] = round_to_byte(blue_total / alpha_total);
    } else {
        target[0] = 0;
        target[1] = 0;
        target[2] = 0;
    }
}

static unsigned char *resize_with_alpha_weight(const unsigned char *source, unsigned int source_width, unsigned int source_height, unsigned int target_width, unsigned int target_height)
{
    unsigned char *target;
    struct span *columns;
    struct span *rows;
    unsigned int y;
    unsigned int x;

    target = malloc((size_t)target_width * target_height * 4);
    columns = make_spans(source_width, target_width);
    rows = make_spans(source_height, target_height);
    if (target == NULL || columns == NULL || rows == NULL) {
        free(target);
        free(columns);
        free(rows);
        return NULL;
    }
    for (y = 0; y < target_height; y++) {
        for (x = 0; x < target_width; x++) {
            unsigned char *pixel = target + ((size_t)y * target_width + x) * 4;

            average_one_pixel(source, source_width, &columns[x], &rows[y], pixel);
        }
    }
    free(columns);
    free(rows);
    return target;
}

static void copy_colour(unsigned char *pixels, size_t from_index, size_t to_index)
{
    pixels[to_index * 4] = pixels[from_index * 4];
    pixels[to_index * 4 + 1] = pixels[from_index * 4 + 1];
    pixels[to_index * 4 + 2] = pixels[from_index * 4 + 2];
}

static int fill_transparent_colours(unsigned char *pixels, unsigned int width, unsigned int height)
{
    size_t pixel_count = (size_t)width * height;
    size_t *queue;
    unsigned char *is_done;
    size_t queue_start = 0;
    size_t queue_end = 0;
    size_t index;

    queue = malloc(sizeof(size_t) * pixel_count);
    is_done = malloc(pixel_count);
    if (queue == NULL || is_done == NULL) {
        free(queue);
        free(is_done);
        return 0;
    }
    for (index = 0; index < pixel_count; index++) {
        is_done[index] = pixels[index * 4 + 3] != 0;
        if (is_done[index]) {
            queue[queue_end] = index;
            queue_end = queue_end + 1;
        }
    }
    while (queue_start < queue_end) {
        size_t current = queue[queue_start];
        unsigned int x = (unsigned int)(current % width);
        unsigned int y = (unsigned int)(current / width);
        size_t neighbours[4];
        int neighbour_count = 0;
        int neighbour;

        queue_start = queue_start + 1;
        if (x > 0) {
            neighbours[neighbour_count] = current - 1;
            neighbour_count = neighbour_count + 1;
        }
        if (x + 1 < width) {
            neighbours[neighbour_count] = current + 1;
            neighbour_count = neighbour_count + 1;
        }
        if (y > 0) {
            neighbours[neighbour_count] = current - width;
            neighbour_count = neighbour_count + 1;
        }
        if (y + 1 < height) {
            neighbours[neighbour_count] = current + width;
            neighbour_count = neighbour_count + 1;
        }
        for (neighbour = 0; neighbour < neighbour_count; neighbour++) {
            size_t next = neighbours[neighbour];

            if (!is_done[next]) {
                is_done[next] = 1;
                copy_colour(pixels, current, next);
                queue[queue_end] = next;
                queue_end = queue_end + 1;
            }
        }
    }
    free(queue);
    free(is_done);
    return 1;
}

static int image_is_fully_opaque(const unsigned char *pixels, unsigned int width, unsigned int height)
{
    size_t pixel_count = (size_t)width * height;
    size_t index;

    for (index = 0; index < pixel_count; index++) {
        if (pixels[index * 4 + 3] != 255) {
            return 0;
        }
    }
    return 1;
}

static int write_whole_file(const char *path, const unsigned char *data, size_t size)
{
    FILE *file;
    size_t written;

    file = fopen(path, "wb");
    if (file == NULL) {
        return 0;
    }
    written = fwrite(data, 1, size, file);
    fclose(file);
    return written == size;
}

static int fail(const char *input_path, const char *message)
{
    fprintf(stderr, "texture_tool: %s: %s\n", input_path, message);
    return 1;
}

static int parse_size_argument(const char *text, unsigned int *value)
{
    char *end;
    long parsed;

    parsed = strtol(text, &end, 10);
    if (end == text || *end != '\0' || parsed < 0 || parsed > TEX_MAX_SIDE) {
        return 0;
    }
    *value = (unsigned int)parsed;
    return 1;
}

static int parse_crop_arguments(char **arguments, const struct tex_image *image, struct rectangle *crop)
{
    if (!parse_size_argument(arguments[0], &crop->x) || !parse_size_argument(arguments[1], &crop->y)) {
        return 0;
    }
    if (!parse_size_argument(arguments[2], &crop->width) || !parse_size_argument(arguments[3], &crop->height)) {
        return 0;
    }
    if (crop->width == 0 || crop->height == 0) {
        return 0;
    }
    return crop->x + crop->width <= image->width && crop->y + crop->height <= image->height;
}

int main(int argument_count, char **arguments)
{
    const char *input_path;
    const char *output_path;
    const char *problem;
    unsigned char *file_data;
    unsigned char *decoded;
    unsigned char *cropped;
    unsigned char *resized;
    size_t file_size;
    struct tex_image image;
    struct rectangle crop;
    unsigned int output_width;
    unsigned int output_height;
    int is_opaque;

    if (argument_count != 5 && argument_count != 9) {
        fprintf(stderr, "usage: texture_tool input.tex output.rgba width height [crop_x crop_y crop_width crop_height]\n");
        return 2;
    }
    input_path = arguments[1];
    output_path = arguments[2];
    if (!parse_size_argument(arguments[3], &output_width) || !parse_size_argument(arguments[4], &output_height)) {
        return fail(input_path, "bad output size");
    }
    if (output_width == 0 || output_height == 0) {
        return fail(input_path, "bad output size");
    }
    file_data = read_whole_file(input_path, &file_size);
    if (file_data == NULL) {
        return fail(input_path, "cannot read the file");
    }
    problem = read_tex_header(file_data, file_size, &image);
    if (problem != NULL) {
        return fail(input_path, problem);
    }
    decoded = decode_top_mip(&image);
    if (decoded == NULL) {
        return fail(input_path, "out of memory");
    }
    crop.x = 0;
    crop.y = 0;
    crop.width = image.width;
    crop.height = image.height;
    cropped = decoded;
    if (argument_count == 9) {
        if (!parse_crop_arguments(&arguments[5], &image, &crop)) {
            return fail(input_path, "bad crop rectangle");
        }
        cropped = crop_pixels(decoded, image.width, &crop);
        if (cropped == NULL) {
            return fail(input_path, "out of memory");
        }
    }
    resized = resize_with_alpha_weight(cropped, crop.width, crop.height, output_width, output_height);
    if (resized == NULL) {
        return fail(input_path, "out of memory");
    }
    is_opaque = image_is_fully_opaque(resized, output_width, output_height);
    if (!fill_transparent_colours(resized, output_width, output_height)) {
        return fail(input_path, "out of memory");
    }
    if (!write_whole_file(output_path, resized, (size_t)output_width * output_height * 4)) {
        return fail(input_path, "cannot write the output file");
    }
    printf("%u %u %u %d\n", image.width, image.height, image.format, is_opaque);
    if (cropped != decoded) {
        free(cropped);
    }
    free(decoded);
    free(resized);
    free(file_data);
    return 0;
}
