#include "astc_decode.h"

#include <math.h>
#include <string.h>

enum {
    PADDED_BLOCK_BYTES = ASTC_BLOCK_BYTES + 4,
    MAX_GRID_WEIGHTS = 64,
    MIN_WEIGHT_BITS = 24,
    MAX_WEIGHT_BITS = 96,
    MAX_COLOR_VALUES = 18,
    PARTITION_SEED_BITS = 10,
    FIRST_COLOR_BIT_SINGLE_PARTITION = 17,
    FIRST_COLOR_BIT_MULTI_PARTITION = 29,
    SMALL_BLOCK_TEXEL_LIMIT = 31,
    ISE_LEVEL_6_INDEX = 4,
    ISE_LEVEL_INDEX_COUNT = 21,
    VOID_EXTENT_MODE_MASK = 0x1FF,
    VOID_EXTENT_MODE_VALUE = 0x1FC,
    VOID_EXTENT_UNCONSTRAINED = 0x1FFF
};

typedef struct {
    int level_count;
    int has_trit;
    int has_quint;
    int bit_count;
} ise_encoding;

typedef struct {
    int digit;
    int low_bits;
} ise_value;

typedef struct {
    int values[2][4];
    int rgb_is_log;
    int alpha_is_log;
} hdr_endpoints;

static const ise_encoding ise_encodings[ISE_LEVEL_INDEX_COUNT] = {
    {2, 0, 0, 1},
    {3, 1, 0, 0},
    {4, 0, 0, 2},
    {5, 0, 1, 0},
    {6, 1, 0, 1},
    {8, 0, 0, 3},
    {10, 0, 1, 1},
    {12, 1, 0, 2},
    {16, 0, 0, 4},
    {20, 0, 1, 2},
    {24, 1, 0, 3},
    {32, 0, 0, 5},
    {40, 0, 1, 3},
    {48, 1, 0, 4},
    {64, 0, 0, 6},
    {80, 0, 1, 4},
    {96, 1, 0, 5},
    {128, 0, 0, 7},
    {160, 0, 1, 5},
    {192, 1, 0, 6},
    {256, 0, 0, 8}
};

static uint32_t read_bits(const uint8_t *bytes, int first_bit, int bit_count)
{
    int byte_index = first_bit >> 3;
    uint32_t window = (uint32_t)bytes[byte_index] | ((uint32_t)bytes[byte_index + 1] << 8) | ((uint32_t)bytes[byte_index + 2] << 16);
    return (window >> (first_bit & 7)) & ((1u << bit_count) - 1u);
}

static uint8_t reverse_byte_bits(uint8_t value)
{
    uint8_t reversed = 0;
    for (int bit = 0; bit < 8; bit++) {
        reversed = (uint8_t)((reversed << 1) | ((value >> bit) & 1));
    }
    return reversed;
}

static int ise_sequence_bit_count(int value_count, const ise_encoding *encoding)
{
    int total = value_count * encoding->bit_count;
    if (encoding->has_trit) {
        total += (8 * value_count + 4) / 5;
    }
    if (encoding->has_quint) {
        total += (7 * value_count + 2) / 3;
    }
    return total;
}

static void expand_trit_group(int packed, int digits[5])
{
    int combined;
    if (((packed >> 2) & 7) == 7) {
        combined = (((packed >> 5) & 7) << 2) | (packed & 3);
        digits[4] = 2;
        digits[3] = 2;
    } else {
        combined = packed & 0x1F;
        if (((packed >> 5) & 3) == 3) {
            digits[4] = 2;
            digits[3] = (packed >> 7) & 1;
        } else {
            digits[4] = (packed >> 7) & 1;
            digits[3] = (packed >> 5) & 3;
        }
    }
    if ((combined & 3) == 3) {
        int bit3 = (combined >> 3) & 1;
        int bit2 = (combined >> 2) & 1;
        digits[2] = 2;
        digits[1] = (combined >> 4) & 1;
        digits[0] = (bit3 << 1) | (bit2 & (bit3 ^ 1));
    } else if (((combined >> 2) & 3) == 3) {
        digits[2] = 2;
        digits[1] = 2;
        digits[0] = combined & 3;
    } else {
        int bit1 = (combined >> 1) & 1;
        int bit0 = combined & 1;
        digits[2] = (combined >> 4) & 1;
        digits[1] = (combined >> 2) & 3;
        digits[0] = (bit1 << 1) | (bit0 & (bit1 ^ 1));
    }
}

static void expand_quint_group(int packed, int digits[3])
{
    int upper_is_three = ((packed >> 1) & 3) == 3;
    int top_field = (packed >> 5) & 3;
    if (upper_is_three && top_field == 0) {
        int bit0 = packed & 1;
        int not_bit0 = bit0 ^ 1;
        int bit4 = (packed >> 4) & 1;
        int bit3 = (packed >> 3) & 1;
        digits[2] = (bit0 << 2) | ((bit4 & not_bit0) << 1) | (bit3 & not_bit0);
        digits[1] = 4;
        digits[0] = 4;
        return;
    }
    int combined;
    if (upper_is_three) {
        digits[2] = 4;
        combined = (((packed >> 3) & 3) << 3) | ((top_field ^ 3) << 1) | (packed & 1);
    } else {
        digits[2] = top_field;
        combined = packed & 0x1F;
    }
    if ((combined & 7) == 5) {
        digits[1] = 4;
        digits[0] = (combined >> 3) & 3;
    } else {
        digits[1] = (combined >> 3) & 3;
        digits[0] = combined & 7;
    }
}

static void decode_ise_sequence(const uint8_t *stream, int first_bit, int value_count, const ise_encoding *encoding, ise_value *values)
{
    static const uint8_t trit_field_bits[5] = {2, 2, 1, 2, 1};
    static const uint8_t trit_field_shift[5] = {0, 2, 4, 5, 7};
    static const uint8_t quint_field_bits[3] = {3, 2, 2};
    static const uint8_t quint_field_shift[3] = {0, 3, 5};
    int bit_position = first_bit;
    int packed_group = 0;
    for (int value_index = 0; value_index < value_count; value_index++) {
        values[value_index].low_bits = (int)read_bits(stream, bit_position, encoding->bit_count);
        values[value_index].digit = 0;
        bit_position += encoding->bit_count;
        if (encoding->has_trit) {
            int group_position = value_index % 5;
            packed_group |= (int)read_bits(stream, bit_position, trit_field_bits[group_position]) << trit_field_shift[group_position];
            bit_position += trit_field_bits[group_position];
            if (group_position == 4 || value_index == value_count - 1) {
                int digits[5];
                expand_trit_group(packed_group, digits);
                for (int member = 0; member <= group_position; member++) {
                    values[value_index - group_position + member].digit = digits[member];
                }
                packed_group = 0;
            }
        } else if (encoding->has_quint) {
            int group_position = value_index % 3;
            packed_group |= (int)read_bits(stream, bit_position, quint_field_bits[group_position]) << quint_field_shift[group_position];
            bit_position += quint_field_bits[group_position];
            if (group_position == 2 || value_index == value_count - 1) {
                int digits[3];
                expand_quint_group(packed_group, digits);
                for (int member = 0; member <= group_position; member++) {
                    values[value_index - group_position + member].digit = digits[member];
                }
                packed_group = 0;
            }
        }
    }
}

static int replicate_bits(int value, int source_bits, int target_bits)
{
    int result = 0;
    int shift = target_bits - source_bits;
    while (shift > -source_bits) {
        if (shift >= 0) {
            result |= value << shift;
        } else {
            result |= value >> -shift;
        }
        shift -= source_bits;
    }
    return result & ((1 << target_bits) - 1);
}

static int unquantize_weight(const ise_encoding *encoding, ise_value value)
{
    int unscaled;
    if (!encoding->has_trit && !encoding->has_quint) {
        unscaled = replicate_bits(value.low_bits, encoding->bit_count, 6);
    } else if (encoding->bit_count == 0) {
        static const uint8_t three_level_weights[3] = {0, 32, 63};
        static const uint8_t five_level_weights[5] = {0, 16, 32, 47, 63};
        unscaled = encoding->has_trit ? three_level_weights[value.digit] : five_level_weights[value.digit];
    } else {
        int lowest_bit_mask = (value.low_bits & 1) ? 0x7F : 0;
        int upper_bits = value.low_bits >> 1;
        int scale = 0;
        int pattern = 0;
        switch (encoding->level_count) {
        case 6:
            scale = 50;
            break;
        case 10:
            scale = 28;
            break;
        case 12:
            scale = 23;
            pattern = (upper_bits << 6) | (upper_bits << 2) | upper_bits;
            break;
        case 20:
            scale = 13;
            pattern = (upper_bits << 6) | (upper_bits << 1);
            break;
        default:
            scale = 11;
            pattern = (upper_bits << 5) | upper_bits;
            break;
        }
        int scrambled = (value.digit * scale + pattern) ^ lowest_bit_mask;
        unscaled = (lowest_bit_mask & 0x20) | (scrambled >> 2);
    }
    return unscaled > 32 ? unscaled + 1 : unscaled;
}

static int unquantize_color(const ise_encoding *encoding, ise_value value)
{
    if (!encoding->has_trit && !encoding->has_quint) {
        return replicate_bits(value.low_bits, encoding->bit_count, 8);
    }
    int lowest_bit_mask = (value.low_bits & 1) ? 0x1FF : 0;
    int upper_bits = value.low_bits >> 1;
    int scale = 0;
    int pattern = 0;
    switch (encoding->level_count) {
    case 6:
        scale = 204;
        break;
    case 10:
        scale = 113;
        break;
    case 12:
        scale = 93;
        pattern = (upper_bits << 8) | (upper_bits << 4) | (upper_bits << 2) | (upper_bits << 1);
        break;
    case 20:
        scale = 54;
        pattern = (upper_bits << 8) | (upper_bits << 3) | (upper_bits << 2);
        break;
    case 24:
        scale = 44;
        pattern = (upper_bits << 7) | (upper_bits << 2) | upper_bits;
        break;
    case 40:
        scale = 26;
        pattern = (upper_bits << 7) | (upper_bits << 1) | (upper_bits >> 1);
        break;
    case 48:
        scale = 22;
        pattern = (upper_bits << 6) | upper_bits;
        break;
    case 80:
        scale = 13;
        pattern = (upper_bits << 6) | (upper_bits >> 1);
        break;
    case 96:
        scale = 11;
        pattern = (upper_bits << 5) | (upper_bits >> 2);
        break;
    case 160:
        scale = 6;
        pattern = (upper_bits << 5) | (upper_bits >> 3);
        break;
    default:
        scale = 5;
        pattern = (upper_bits << 4) | (upper_bits >> 4);
        break;
    }
    int scrambled = (value.digit * scale + pattern) ^ lowest_bit_mask;
    return (lowest_bit_mask & 0x80) | (scrambled >> 2);
}

static int decode_block_mode(int mode_bits, astc_block_description *description)
{
    int range_bits = (mode_bits >> 4) & 1;
    int high_precision = (mode_bits >> 9) & 1;
    int dual_plane = (mode_bits >> 10) & 1;
    int field_a = (mode_bits >> 5) & 3;
    int grid_width = 0;
    int grid_height = 0;
    if ((mode_bits & 3) != 0) {
        int field_b = (mode_bits >> 7) & 3;
        range_bits |= (mode_bits & 3) << 1;
        switch ((mode_bits >> 2) & 3) {
        case 0:
            grid_width = field_b + 4;
            grid_height = field_a + 2;
            break;
        case 1:
            grid_width = field_b + 8;
            grid_height = field_a + 2;
            break;
        case 2:
            grid_width = field_a + 2;
            grid_height = field_b + 8;
            break;
        default:
            field_b &= 1;
            if (mode_bits & 0x100) {
                grid_width = field_b + 2;
                grid_height = field_a + 2;
            } else {
                grid_width = field_a + 2;
                grid_height = field_b + 6;
            }
            break;
        }
    } else {
        int range_upper = (mode_bits >> 2) & 3;
        if (range_upper == 0) {
            return 0;
        }
        range_bits |= range_upper << 1;
        switch ((mode_bits >> 7) & 3) {
        case 0:
            grid_width = 12;
            grid_height = field_a + 2;
            break;
        case 1:
            grid_width = field_a + 2;
            grid_height = 12;
            break;
        case 2:
            grid_width = field_a + 6;
            grid_height = ((mode_bits >> 9) & 3) + 6;
            dual_plane = 0;
            high_precision = 0;
            break;
        default:
            if (field_a == 0) {
                grid_width = 6;
                grid_height = 10;
            } else if (field_a == 1) {
                grid_width = 10;
                grid_height = 6;
            } else {
                return 0;
            }
            break;
        }
    }
    int weight_level_index = (range_bits - 2) + 6 * high_precision;
    if (weight_level_index < 0 || weight_level_index > 11) {
        return 0;
    }
    int weight_count = grid_width * grid_height * (dual_plane + 1);
    if (weight_count > MAX_GRID_WEIGHTS) {
        return 0;
    }
    int weight_bit_count = ise_sequence_bit_count(weight_count, &ise_encodings[weight_level_index]);
    if (weight_bit_count < MIN_WEIGHT_BITS || weight_bit_count > MAX_WEIGHT_BITS) {
        return 0;
    }
    description->grid_width = grid_width;
    description->grid_height = grid_height;
    description->is_dual_plane = dual_plane;
    description->weight_level_count = ise_encodings[weight_level_index].level_count;
    description->weight_bit_count = weight_bit_count;
    return 1;
}

static const ise_encoding *find_ise_encoding(int level_count)
{
    for (int index = 0; index < ISE_LEVEL_INDEX_COUNT; index++) {
        if (ise_encodings[index].level_count == level_count) {
            return &ise_encodings[index];
        }
    }
    return &ise_encodings[0];
}

static void describe_void_extent(const uint8_t *padded_block, astc_block_description *description)
{
    int low_s = (int)read_bits(padded_block, 12, 13);
    int high_s = (int)read_bits(padded_block, 25, 13);
    int low_t = (int)read_bits(padded_block, 38, 13);
    int high_t = (int)read_bits(padded_block, 51, 13);
    int unconstrained = low_s == VOID_EXTENT_UNCONSTRAINED && high_s == VOID_EXTENT_UNCONSTRAINED && low_t == VOID_EXTENT_UNCONSTRAINED && high_t == VOID_EXTENT_UNCONSTRAINED;
    int reserved_bits_valid = read_bits(padded_block, 10, 2) == 3;
    int is_hdr = (int)read_bits(padded_block, 9, 1);
    if (!reserved_bits_valid || (!unconstrained && (low_s >= high_s || low_t >= high_t))) {
        description->kind = ASTC_BLOCK_ILLEGAL;
        return;
    }
    description->kind = is_hdr ? ASTC_BLOCK_VOID_EXTENT_HDR : ASTC_BLOCK_VOID_EXTENT_LDR;
}

static void describe_padded_block(const uint8_t *padded_block, int block_width, int block_height, astc_block_description *description)
{
    int mode_bits = (int)read_bits(padded_block, 0, 11);
    memset(description, 0, sizeof(*description));
    description->kind = ASTC_BLOCK_ILLEGAL;
    if ((mode_bits & VOID_EXTENT_MODE_MASK) == VOID_EXTENT_MODE_VALUE) {
        describe_void_extent(padded_block, description);
        return;
    }
    if (!decode_block_mode(mode_bits, description)) {
        return;
    }
    if (description->grid_width > block_width || description->grid_height > block_height) {
        return;
    }
    int partition_count = (int)read_bits(padded_block, 11, 2) + 1;
    description->partition_count = partition_count;
    if (partition_count == 4 && description->is_dual_plane) {
        return;
    }
    int bits_below_weights = 128 - description->weight_bit_count;
    int extra_mode_bits = 0;
    if (partition_count == 1) {
        description->endpoint_modes[0] = (int)read_bits(padded_block, 13, 4);
        description->color_first_bit = FIRST_COLOR_BIT_SINGLE_PARTITION;
    } else {
        description->partition_seed = (int)read_bits(padded_block, 13, PARTITION_SEED_BITS);
        description->color_first_bit = FIRST_COLOR_BIT_MULTI_PARTITION;
        int low_mode_field = (int)read_bits(padded_block, 23, 6);
        int selector = low_mode_field & 3;
        if (selector == 0) {
            for (int partition = 0; partition < partition_count; partition++) {
                description->endpoint_modes[partition] = (low_mode_field >> 2) & 0xF;
            }
        } else {
            extra_mode_bits = 3 * partition_count - 4;
            int mode_field = low_mode_field | ((int)read_bits(padded_block, bits_below_weights - extra_mode_bits, extra_mode_bits) << 6);
            int base_class = selector - 1;
            for (int partition = 0; partition < partition_count; partition++) {
                int class_offset = (mode_field >> (2 + partition)) & 1;
                int mode_low_bits = (mode_field >> (2 + partition_count + 2 * partition)) & 3;
                description->endpoint_modes[partition] = ((base_class + class_offset) << 2) | mode_low_bits;
            }
        }
    }
    int color_value_count = 0;
    for (int partition = 0; partition < partition_count; partition++) {
        int mode = description->endpoint_modes[partition];
        color_value_count += ((mode >> 2) + 1) * 2;
        if (mode == 2 || mode == 3 || mode == 7 || mode == 11 || mode == 14 || mode == 15) {
            description->uses_hdr_endpoint_mode = 1;
        }
    }
    if (color_value_count > MAX_COLOR_VALUES) {
        return;
    }
    description->color_value_count = color_value_count;
    int color_bits_available = bits_below_weights - extra_mode_bits - description->color_first_bit;
    if (description->is_dual_plane) {
        description->plane2_component = (int)read_bits(padded_block, bits_below_weights - extra_mode_bits - 2, 2);
        color_bits_available -= 2;
    }
    for (int index = ISE_LEVEL_INDEX_COUNT - 1; index >= ISE_LEVEL_6_INDEX; index--) {
        if (ise_sequence_bit_count(color_value_count, &ise_encodings[index]) <= color_bits_available) {
            description->color_level_count = ise_encodings[index].level_count;
            description->kind = ASTC_BLOCK_NORMAL;
            return;
        }
    }
}

void astc_describe_block(const uint8_t *block, int block_width, int block_height, astc_block_description *description)
{
    uint8_t padded_block[PADDED_BLOCK_BYTES] = {0};
    memcpy(padded_block, block, ASTC_BLOCK_BYTES);
    describe_padded_block(padded_block, block_width, block_height, description);
}

static uint32_t hash_partition_seed(uint32_t seed)
{
    seed ^= seed >> 15;
    seed -= seed << 17;
    seed += seed << 7;
    seed += seed << 4;
    seed ^= seed >> 5;
    seed += seed << 16;
    seed ^= seed >> 7;
    seed ^= seed >> 3;
    seed ^= seed << 6;
    seed ^= seed >> 17;
    return seed;
}

static int select_partition(int partition_seed, int x, int y, int partition_count, int is_small_block)
{
    if (is_small_block) {
        x <<= 1;
        y <<= 1;
    }
    int seed = partition_seed + (partition_count - 1) * 1024;
    uint32_t random_bits = hash_partition_seed((uint32_t)seed);
    int factors[8];
    for (int index = 0; index < 8; index++) {
        int nibble = (int)((random_bits >> (index * 4)) & 0xF);
        factors[index] = nibble * nibble;
    }
    int shift_first;
    int shift_second;
    if (seed & 1) {
        shift_first = (seed & 2) ? 4 : 5;
        shift_second = (partition_count == 3) ? 6 : 5;
    } else {
        shift_first = (partition_count == 3) ? 6 : 5;
        shift_second = (seed & 2) ? 4 : 5;
    }
    for (int index = 0; index < 8; index++) {
        factors[index] >>= (index & 1) ? shift_second : shift_first;
    }
    int score_a = (factors[0] * x + factors[1] * y + (int)(random_bits >> 14)) & 0x3F;
    int score_b = (factors[2] * x + factors[3] * y + (int)(random_bits >> 10)) & 0x3F;
    int score_c = (factors[4] * x + factors[5] * y + (int)(random_bits >> 6)) & 0x3F;
    int score_d = (factors[6] * x + factors[7] * y + (int)(random_bits >> 2)) & 0x3F;
    if (partition_count < 4) {
        score_d = 0;
    }
    if (partition_count < 3) {
        score_c = 0;
    }
    if (score_a >= score_b && score_a >= score_c && score_a >= score_d) {
        return 0;
    }
    if (score_b >= score_c && score_b >= score_d) {
        return 1;
    }
    return score_c >= score_d ? 2 : 3;
}

static void transfer_precision_signed(int *offset_value, int *base_value)
{
    *base_value = (*base_value >> 1) | (*offset_value & 0x80);
    *offset_value = (*offset_value >> 1) & 0x3F;
    if (*offset_value & 0x20) {
        *offset_value -= 0x40;
    }
}

static int clamp_to_unorm8(int value)
{
    if (value < 0) {
        return 0;
    }
    return value > 255 ? 255 : value;
}

static void set_endpoint(int *endpoint, int red, int green, int blue, int alpha)
{
    endpoint[0] = clamp_to_unorm8(red);
    endpoint[1] = clamp_to_unorm8(green);
    endpoint[2] = clamp_to_unorm8(blue);
    endpoint[3] = clamp_to_unorm8(alpha);
}

static void set_endpoint_blue_contracted(int *endpoint, int red, int green, int blue, int alpha)
{
    set_endpoint(endpoint, (red + blue) >> 1, (green + blue) >> 1, blue, alpha);
}

static void decode_endpoint_direct_rgba(const int *values, int alpha0, int alpha1, int *endpoint0, int *endpoint1)
{
    int sum0 = values[0] + values[2] + values[4];
    int sum1 = values[1] + values[3] + values[5];
    if (sum1 >= sum0) {
        set_endpoint(endpoint0, values[0], values[2], values[4], alpha0);
        set_endpoint(endpoint1, values[1], values[3], values[5], alpha1);
    } else {
        set_endpoint_blue_contracted(endpoint0, values[1], values[3], values[5], alpha1);
        set_endpoint_blue_contracted(endpoint1, values[0], values[2], values[4], alpha0);
    }
}

static void decode_endpoint_offset_rgba(int *values, int has_alpha, int *endpoint0, int *endpoint1)
{
    transfer_precision_signed(&values[1], &values[0]);
    transfer_precision_signed(&values[3], &values[2]);
    transfer_precision_signed(&values[5], &values[4]);
    int alpha0 = 255;
    int alpha1 = 255;
    if (has_alpha) {
        transfer_precision_signed(&values[7], &values[6]);
        alpha0 = values[6];
        alpha1 = values[6] + values[7];
    }
    if (values[1] + values[3] + values[5] >= 0) {
        set_endpoint(endpoint0, values[0], values[2], values[4], alpha0);
        set_endpoint(endpoint1, values[0] + values[1], values[2] + values[3], values[4] + values[5], alpha1);
    } else {
        set_endpoint_blue_contracted(endpoint0, values[0] + values[1], values[2] + values[3], values[4] + values[5], alpha1);
        set_endpoint_blue_contracted(endpoint1, values[0], values[2], values[4], alpha0);
    }
}

static int decode_ldr_endpoints(int endpoint_mode, int *values, int *endpoint0, int *endpoint1)
{
    switch (endpoint_mode) {
    case 0:
        set_endpoint(endpoint0, values[0], values[0], values[0], 255);
        set_endpoint(endpoint1, values[1], values[1], values[1], 255);
        return 1;
    case 1: {
        int low = (values[0] >> 2) | (values[1] & 0xC0);
        int high = low + (values[1] & 0x3F);
        set_endpoint(endpoint0, low, low, low, 255);
        set_endpoint(endpoint1, high, high, high, 255);
        return 1;
    }
    case 4:
        set_endpoint(endpoint0, values[0], values[0], values[0], values[2]);
        set_endpoint(endpoint1, values[1], values[1], values[1], values[3]);
        return 1;
    case 5:
        transfer_precision_signed(&values[1], &values[0]);
        transfer_precision_signed(&values[3], &values[2]);
        set_endpoint(endpoint0, values[0], values[0], values[0], values[2]);
        set_endpoint(endpoint1, values[0] + values[1], values[0] + values[1], values[0] + values[1], values[2] + values[3]);
        return 1;
    case 6:
        set_endpoint(endpoint0, (values[0] * values[3]) >> 8, (values[1] * values[3]) >> 8, (values[2] * values[3]) >> 8, 255);
        set_endpoint(endpoint1, values[0], values[1], values[2], 255);
        return 1;
    case 8:
        decode_endpoint_direct_rgba(values, 255, 255, endpoint0, endpoint1);
        return 1;
    case 9:
        decode_endpoint_offset_rgba(values, 0, endpoint0, endpoint1);
        return 1;
    case 10:
        set_endpoint(endpoint0, (values[0] * values[3]) >> 8, (values[1] * values[3]) >> 8, (values[2] * values[3]) >> 8, values[4]);
        set_endpoint(endpoint1, values[0], values[1], values[2], values[5]);
        return 1;
    case 12:
        decode_endpoint_direct_rgba(values, values[6], values[7], endpoint0, endpoint1);
        return 1;
    case 13:
        decode_endpoint_offset_rgba(values, 1, endpoint0, endpoint1);
        return 1;
    default:
        return 0;
    }
}

static void fill_error_color(int texel_count, uint16_t *texels_rgba16)
{
    for (int texel = 0; texel < texel_count; texel++) {
        texels_rgba16[texel * 4 + 0] = 0xFFFF;
        texels_rgba16[texel * 4 + 1] = 0;
        texels_rgba16[texel * 4 + 2] = 0xFFFF;
        texels_rgba16[texel * 4 + 3] = 0xFFFF;
    }
}

static void decode_grid_weights(const uint8_t *padded_block, const astc_block_description *description, int *plane0_weights, int *plane1_weights)
{
    uint8_t reversed_block[PADDED_BLOCK_BYTES] = {0};
    for (int byte_index = 0; byte_index < ASTC_BLOCK_BYTES; byte_index++) {
        reversed_block[byte_index] = reverse_byte_bits(padded_block[ASTC_BLOCK_BYTES - 1 - byte_index]);
    }
    int grid_count = description->grid_width * description->grid_height;
    int weight_count = grid_count * (description->is_dual_plane + 1);
    const ise_encoding *encoding = find_ise_encoding(description->weight_level_count);
    ise_value weight_values[MAX_GRID_WEIGHTS];
    decode_ise_sequence(reversed_block, 0, weight_count, encoding, weight_values);
    for (int index = 0; index < grid_count; index++) {
        if (description->is_dual_plane) {
            plane0_weights[index] = unquantize_weight(encoding, weight_values[index * 2]);
            plane1_weights[index] = unquantize_weight(encoding, weight_values[index * 2 + 1]);
        } else {
            plane0_weights[index] = unquantize_weight(encoding, weight_values[index]);
            plane1_weights[index] = 0;
        }
    }
}

static int interpolate_grid_weight(const int *grid_weights, int grid_width, int grid_height, int base_x, int base_y, int fraction_x, int fraction_y)
{
    int next_x = base_x + 1 < grid_width ? base_x + 1 : base_x;
    int next_y = base_y + 1 < grid_height ? base_y + 1 : base_y;
    int weight_both = (fraction_x * fraction_y + 8) >> 4;
    int weight_y_only = fraction_y - weight_both;
    int weight_x_only = fraction_x - weight_both;
    int weight_none = 16 - fraction_x - fraction_y + weight_both;
    int sum = grid_weights[base_x + base_y * grid_width] * weight_none
        + grid_weights[next_x + base_y * grid_width] * weight_x_only
        + grid_weights[base_x + next_y * grid_width] * weight_y_only
        + grid_weights[next_x + next_y * grid_width] * weight_both;
    return (sum + 8) >> 4;
}

typedef struct {
    astc_block_description description;
    int partition_values[4][8];
    int plane0_weights[MAX_GRID_WEIGHTS];
    int plane1_weights[MAX_GRID_WEIGHTS];
    int scale_x;
    int scale_y;
    int is_small_block;
} prepared_block;

static void prepare_normal_block(const uint8_t *padded_block, int block_width, int block_height, prepared_block *prepared)
{
    const astc_block_description *description = &prepared->description;
    ise_value color_values[MAX_COLOR_VALUES];
    const ise_encoding *color_encoding = find_ise_encoding(description->color_level_count);
    decode_ise_sequence(padded_block, description->color_first_bit, description->color_value_count, color_encoding, color_values);
    int value_position = 0;
    for (int partition = 0; partition < description->partition_count; partition++) {
        int mode_value_count = ((description->endpoint_modes[partition] >> 2) + 1) * 2;
        for (int index = 0; index < mode_value_count; index++) {
            prepared->partition_values[partition][index] = unquantize_color(color_encoding, color_values[value_position + index]);
        }
        value_position += mode_value_count;
    }
    decode_grid_weights(padded_block, description, prepared->plane0_weights, prepared->plane1_weights);
    prepared->scale_x = (1024 + block_width / 2) / (block_width - 1);
    prepared->scale_y = (1024 + block_height / 2) / (block_height - 1);
    prepared->is_small_block = block_width * block_height < SMALL_BLOCK_TEXEL_LIMIT;
}

static int texel_partition(const prepared_block *prepared, int x, int y)
{
    if (prepared->description.partition_count == 1) {
        return 0;
    }
    return select_partition(prepared->description.partition_seed, x, y, prepared->description.partition_count, prepared->is_small_block);
}

static void texel_weights(const prepared_block *prepared, int x, int y, int *plane0_weight, int *plane1_weight)
{
    const astc_block_description *description = &prepared->description;
    int grid_position_x = (prepared->scale_x * x * (description->grid_width - 1) + 32) >> 6;
    int grid_position_y = (prepared->scale_y * y * (description->grid_height - 1) + 32) >> 6;
    int base_x = grid_position_x >> 4;
    int base_y = grid_position_y >> 4;
    int fraction_x = grid_position_x & 0xF;
    int fraction_y = grid_position_y & 0xF;
    *plane0_weight = interpolate_grid_weight(prepared->plane0_weights, description->grid_width, description->grid_height, base_x, base_y, fraction_x, fraction_y);
    *plane1_weight = 0;
    if (description->is_dual_plane) {
        *plane1_weight = interpolate_grid_weight(prepared->plane1_weights, description->grid_width, description->grid_height, base_x, base_y, fraction_x, fraction_y);
    }
}

astc_block_kind astc_decode_block(const uint8_t *block, int block_width, int block_height, int use_srgb, uint16_t *texels_rgba16)
{
    uint8_t padded_block[PADDED_BLOCK_BYTES] = {0};
    memcpy(padded_block, block, ASTC_BLOCK_BYTES);
    prepared_block prepared;
    astc_block_description *description = &prepared.description;
    describe_padded_block(padded_block, block_width, block_height, description);
    int texel_count = block_width * block_height;
    if (description->kind == ASTC_BLOCK_ILLEGAL || description->kind == ASTC_BLOCK_VOID_EXTENT_HDR) {
        fill_error_color(texel_count, texels_rgba16);
        return description->kind;
    }
    if (description->kind == ASTC_BLOCK_VOID_EXTENT_LDR) {
        for (int texel = 0; texel < texel_count; texel++) {
            for (int channel = 0; channel < 4; channel++) {
                int byte_index = 8 + channel * 2;
                texels_rgba16[texel * 4 + channel] = (uint16_t)(block[byte_index] | (block[byte_index + 1] << 8));
            }
        }
        return description->kind;
    }
    prepare_normal_block(padded_block, block_width, block_height, &prepared);
    int endpoints[4][2][4];
    int partition_is_error[4] = {0, 0, 0, 0};
    for (int partition = 0; partition < description->partition_count; partition++) {
        if (!decode_ldr_endpoints(description->endpoint_modes[partition], prepared.partition_values[partition], endpoints[partition][0], endpoints[partition][1])) {
            partition_is_error[partition] = 1;
        }
    }
    for (int y = 0; y < block_height; y++) {
        for (int x = 0; x < block_width; x++) {
            uint16_t *texel = texels_rgba16 + (x + y * block_width) * 4;
            int partition = texel_partition(&prepared, x, y);
            if (partition_is_error[partition]) {
                texel[0] = 0xFFFF;
                texel[1] = 0;
                texel[2] = 0xFFFF;
                texel[3] = 0xFFFF;
                continue;
            }
            int plane0_weight;
            int plane1_weight;
            texel_weights(&prepared, x, y, &plane0_weight, &plane1_weight);
            for (int channel = 0; channel < 4; channel++) {
                int weight = (description->is_dual_plane && channel == description->plane2_component) ? plane1_weight : plane0_weight;
                int low_endpoint = endpoints[partition][0][channel];
                int high_endpoint = endpoints[partition][1][channel];
                if (use_srgb && channel < 3) {
                    low_endpoint = (low_endpoint << 8) | 0x80;
                    high_endpoint = (high_endpoint << 8) | 0x80;
                } else {
                    low_endpoint *= 257;
                    high_endpoint *= 257;
                }
                texel[channel] = (uint16_t)((low_endpoint * (64 - weight) + high_endpoint * weight + 32) >> 6);
            }
        }
    }
    return description->kind;
}

static void set_hdr_endpoints_rgb(hdr_endpoints *endpoints, int red0, int green0, int blue0, int red1, int green1, int blue1)
{
    endpoints->values[0][0] = red0 << 4;
    endpoints->values[0][1] = green0 << 4;
    endpoints->values[0][2] = blue0 << 4;
    endpoints->values[1][0] = red1 << 4;
    endpoints->values[1][1] = green1 << 4;
    endpoints->values[1][2] = blue1 << 4;
    endpoints->values[0][3] = 0x7800;
    endpoints->values[1][3] = 0x7800;
    endpoints->rgb_is_log = 1;
    endpoints->alpha_is_log = 1;
}

static void decode_hdr_luminance_large_range(const int *values, hdr_endpoints *endpoints)
{
    int luminance0;
    int luminance1;
    if (values[1] >= values[0]) {
        luminance0 = values[0] << 4;
        luminance1 = values[1] << 4;
    } else {
        luminance0 = (values[1] << 4) + 8;
        luminance1 = (values[0] << 4) - 8;
    }
    set_hdr_endpoints_rgb(endpoints, luminance0, luminance0, luminance0, luminance1, luminance1, luminance1);
}

static void decode_hdr_luminance_small_range(const int *values, hdr_endpoints *endpoints)
{
    int luminance0;
    int delta;
    if (values[0] & 0x80) {
        luminance0 = ((values[1] & 0xE0) << 4) | ((values[0] & 0x7F) << 2);
        delta = (values[1] & 0x1F) << 2;
    } else {
        luminance0 = ((values[1] & 0xF0) << 4) | ((values[0] & 0x7F) << 1);
        delta = (values[1] & 0x0F) << 1;
    }
    int luminance1 = luminance0 + delta;
    if (luminance1 > 0xFFF) {
        luminance1 = 0xFFF;
    }
    set_hdr_endpoints_rgb(endpoints, luminance0, luminance0, luminance0, luminance1, luminance1, luminance1);
}

static int clamp_to_12_bits(int value)
{
    if (value < 0) {
        return 0;
    }
    return value > 0xFFF ? 0xFFF : value;
}

static void swap_values(int *first, int *second)
{
    int held = *first;
    *first = *second;
    *second = held;
}

static void decode_hdr_rgb_base_scale(const int *values, hdr_endpoints *endpoints)
{
    int mode_value = ((values[0] & 0xC0) >> 6) | (((values[1] & 0x80) >> 7) << 2) | (((values[2] & 0x80) >> 7) << 3);
    int major_component;
    int mode;
    if ((mode_value & 0xC) != 0xC) {
        major_component = mode_value >> 2;
        mode = mode_value & 3;
    } else if (mode_value != 0xF) {
        major_component = mode_value & 3;
        mode = 4;
    } else {
        major_component = 0;
        mode = 5;
    }
    int red = values[0] & 0x3F;
    int green = values[1] & 0x1F;
    int blue = values[2] & 0x1F;
    int scale = values[3] & 0x1F;
    int bit0 = (values[1] >> 6) & 1;
    int bit1 = (values[1] >> 5) & 1;
    int bit2 = (values[2] >> 6) & 1;
    int bit3 = (values[2] >> 5) & 1;
    int bit4 = (values[3] >> 7) & 1;
    int bit5 = (values[3] >> 6) & 1;
    int bit6 = (values[3] >> 5) & 1;
    int mode_mask = 1 << mode;
    if (mode_mask & 0x30) {
        green |= bit0 << 6;
        blue |= bit2 << 6;
    }
    if (mode_mask & 0x3A) {
        green |= bit1 << 5;
        blue |= bit3 << 5;
    }
    if (mode_mask & 0x3D) {
        scale |= bit6 << 5;
    }
    if (mode_mask & 0x2D) {
        scale |= bit5 << 6;
    }
    if (mode_mask & 0x04) {
        scale |= bit4 << 7;
        red |= bit3 << 6;
    }
    if (mode_mask & 0x3B) {
        red |= bit4 << 6;
    }
    if (mode_mask & 0x10) {
        red |= bit5 << 7;
    }
    if (mode_mask & 0x0F) {
        red |= bit2 << 7;
    }
    if (mode_mask & 0x05) {
        red |= bit1 << 8;
        red |= bit0 << 9;
    }
    if (mode_mask & 0x0A) {
        red |= bit0 << 8;
    }
    if (mode_mask & 0x02) {
        red |= bit6 << 9;
        red |= bit5 << 10;
    }
    if (mode_mask & 0x01) {
        red |= bit3 << 10;
    }
    static const int shift_by_mode[6] = {1, 1, 2, 3, 4, 5};
    int shift = shift_by_mode[mode];
    red <<= shift;
    green <<= shift;
    blue <<= shift;
    scale <<= shift;
    if (mode != 5) {
        green = red - green;
        blue = red - blue;
    }
    if (major_component == 1) {
        swap_values(&red, &green);
    } else if (major_component == 2) {
        swap_values(&red, &blue);
    }
    set_hdr_endpoints_rgb(endpoints, clamp_to_12_bits(red - scale), clamp_to_12_bits(green - scale), clamp_to_12_bits(blue - scale), clamp_to_12_bits(red), clamp_to_12_bits(green), clamp_to_12_bits(blue));
}

static void decode_hdr_rgb_direct(const int *values, hdr_endpoints *endpoints)
{
    int mode_value = ((values[1] & 0x80) >> 7) | (((values[2] & 0x80) >> 7) << 1) | (((values[3] & 0x80) >> 7) << 2);
    int major_component = ((values[4] & 0x80) >> 7) | (((values[5] & 0x80) >> 7) << 1);
    if (major_component == 3) {
        set_hdr_endpoints_rgb(endpoints, values[0] << 4, values[2] << 4, (values[4] & 0x7F) << 5, values[1] << 4, values[3] << 4, (values[5] & 0x7F) << 5);
        return;
    }
    int a = values[0] | ((values[1] & 0x40) << 2);
    int b0 = values[2] & 0x3F;
    int b1 = values[3] & 0x3F;
    int c = values[1] & 0x3F;
    int d0 = values[4] & 0x7F;
    int d1 = values[5] & 0x7F;
    static const int delta_bits_by_mode[8] = {7, 6, 7, 6, 5, 6, 5, 6};
    int delta_bits = delta_bits_by_mode[mode_value];
    int bit0 = (values[2] >> 6) & 1;
    int bit1 = (values[3] >> 6) & 1;
    int bit2 = (values[4] >> 6) & 1;
    int bit3 = (values[5] >> 6) & 1;
    int bit4 = (values[4] >> 5) & 1;
    int bit5 = (values[5] >> 5) & 1;
    int mode_mask = 1 << mode_value;
    if (mode_mask & 0xA4) {
        a |= bit0 << 9;
    }
    if (mode_mask & 0x08) {
        a |= bit2 << 9;
    }
    if (mode_mask & 0x50) {
        a |= bit4 << 9;
        a |= bit5 << 10;
    }
    if (mode_mask & 0xA0) {
        a |= bit1 << 10;
    }
    if (mode_mask & 0xC0) {
        a |= bit2 << 11;
    }
    if (mode_mask & 0x04) {
        c |= bit1 << 6;
    }
    if (mode_mask & 0xE8) {
        c |= bit3 << 6;
    }
    if (mode_mask & 0x20) {
        c |= bit2 << 7;
    }
    if (mode_mask & 0x5B) {
        b0 |= bit0 << 6;
        b1 |= bit1 << 6;
    }
    if (mode_mask & 0x12) {
        b0 |= bit2 << 7;
        b1 |= bit3 << 7;
    }
    if (mode_mask & 0xAF) {
        d0 |= bit4 << 5;
        d1 |= bit5 << 5;
    }
    if (mode_mask & 0x05) {
        d0 |= bit2 << 6;
        d1 |= bit3 << 6;
    }
    int delta_mask = (1 << delta_bits) - 1;
    int sign_bit = 1 << (delta_bits - 1);
    d0 = ((d0 & delta_mask) ^ sign_bit) - sign_bit;
    d1 = ((d1 & delta_mask) ^ sign_bit) - sign_bit;
    int shift = (mode_value >> 1) ^ 3;
    a <<= shift;
    b0 <<= shift;
    b1 <<= shift;
    c <<= shift;
    d0 *= 1 << shift;
    d1 *= 1 << shift;
    int red1 = clamp_to_12_bits(a);
    int green1 = clamp_to_12_bits(a - b0);
    int blue1 = clamp_to_12_bits(a - b1);
    int red0 = clamp_to_12_bits(a - c);
    int green0 = clamp_to_12_bits(a - b0 - c - d0);
    int blue0 = clamp_to_12_bits(a - b1 - c - d1);
    if (major_component == 1) {
        swap_values(&red0, &green0);
        swap_values(&red1, &green1);
    } else if (major_component == 2) {
        swap_values(&red0, &blue0);
        swap_values(&red1, &blue1);
    }
    set_hdr_endpoints_rgb(endpoints, red0, green0, blue0, red1, green1, blue1);
}

static void decode_hdr_alpha(int value6, int value7, hdr_endpoints *endpoints)
{
    int selector = ((value6 >> 7) & 1) | ((value7 >> 6) & 2);
    value6 &= 0x7F;
    value7 &= 0x7F;
    if (selector == 3) {
        value6 <<= 5;
        value7 <<= 5;
    } else {
        value6 |= (value7 << (selector + 1)) & 0x780;
        value7 &= 0x3F >> selector;
        value7 ^= 32 >> selector;
        value7 -= 32 >> selector;
        value6 <<= 4 - selector;
        value7 *= 1 << (4 - selector);
        value7 = clamp_to_12_bits(value7 + value6);
    }
    endpoints->values[0][3] = value6 << 4;
    endpoints->values[1][3] = value7 << 4;
    endpoints->alpha_is_log = 1;
}

static void decode_endpoints_for_hdr_profile(int endpoint_mode, int *values, hdr_endpoints *endpoints)
{
    switch (endpoint_mode) {
    case 2:
        decode_hdr_luminance_large_range(values, endpoints);
        return;
    case 3:
        decode_hdr_luminance_small_range(values, endpoints);
        return;
    case 7:
        decode_hdr_rgb_base_scale(values, endpoints);
        return;
    case 11:
        decode_hdr_rgb_direct(values, endpoints);
        return;
    case 14:
        decode_hdr_rgb_direct(values, endpoints);
        endpoints->values[0][3] = values[6] * 257;
        endpoints->values[1][3] = values[7] * 257;
        endpoints->alpha_is_log = 0;
        return;
    case 15:
        decode_hdr_rgb_direct(values, endpoints);
        decode_hdr_alpha(values[6], values[7], endpoints);
        return;
    default: {
        int low[4];
        int high[4];
        decode_ldr_endpoints(endpoint_mode, values, low, high);
        for (int channel = 0; channel < 4; channel++) {
            endpoints->values[0][channel] = low[channel] * 257;
            endpoints->values[1][channel] = high[channel] * 257;
        }
        endpoints->rgb_is_log = 0;
        endpoints->alpha_is_log = 0;
        return;
    }
    }
}

static float half_bits_to_float(int half_bits)
{
    int exponent = (half_bits >> 10) & 0x1F;
    int mantissa = half_bits & 0x3FF;
    float sign = (half_bits & 0x8000) ? -1.0f : 1.0f;
    if (exponent == 0) {
        return sign * ldexpf((float)mantissa, -24);
    }
    if (exponent == 31) {
        return mantissa == 0 ? sign * HUGE_VALF : NAN;
    }
    return sign * ldexpf((float)(mantissa + 1024), exponent - 25);
}

static float log_value_to_float(int value)
{
    int exponent = (value & 0xF800) >> 11;
    int mantissa = value & 0x7FF;
    int adjusted;
    if (mantissa < 512) {
        adjusted = mantissa * 3;
    } else if (mantissa >= 1536) {
        adjusted = mantissa * 5 - 2048;
    } else {
        adjusted = mantissa * 4 - 512;
    }
    int half_bits = (exponent << 10) + (adjusted >> 3);
    if (half_bits >= 0x7C00) {
        half_bits = 0x7BFF;
    }
    return half_bits_to_float(half_bits);
}

astc_block_kind astc_decode_block_hdr(const uint8_t *block, int block_width, int block_height, float *texels_rgba)
{
    uint8_t padded_block[PADDED_BLOCK_BYTES] = {0};
    memcpy(padded_block, block, ASTC_BLOCK_BYTES);
    prepared_block prepared;
    astc_block_description *description = &prepared.description;
    describe_padded_block(padded_block, block_width, block_height, description);
    int texel_count = block_width * block_height;
    if (description->kind == ASTC_BLOCK_ILLEGAL) {
        for (int index = 0; index < texel_count * 4; index++) {
            texels_rgba[index] = NAN;
        }
        return description->kind;
    }
    if (description->kind != ASTC_BLOCK_NORMAL) {
        for (int texel = 0; texel < texel_count; texel++) {
            for (int channel = 0; channel < 4; channel++) {
                int byte_index = 8 + channel * 2;
                int stored = block[byte_index] | (block[byte_index + 1] << 8);
                texels_rgba[texel * 4 + channel] = description->kind == ASTC_BLOCK_VOID_EXTENT_HDR ? half_bits_to_float(stored) : (float)stored / 65535.0f;
            }
        }
        return description->kind;
    }
    prepare_normal_block(padded_block, block_width, block_height, &prepared);
    hdr_endpoints endpoints[4];
    for (int partition = 0; partition < description->partition_count; partition++) {
        decode_endpoints_for_hdr_profile(description->endpoint_modes[partition], prepared.partition_values[partition], &endpoints[partition]);
    }
    for (int y = 0; y < block_height; y++) {
        for (int x = 0; x < block_width; x++) {
            float *texel = texels_rgba + (x + y * block_width) * 4;
            const hdr_endpoints *partition_endpoints = &endpoints[texel_partition(&prepared, x, y)];
            int plane0_weight;
            int plane1_weight;
            texel_weights(&prepared, x, y, &plane0_weight, &plane1_weight);
            for (int channel = 0; channel < 4; channel++) {
                int weight = (description->is_dual_plane && channel == description->plane2_component) ? plane1_weight : plane0_weight;
                int value = (partition_endpoints->values[0][channel] * (64 - weight) + partition_endpoints->values[1][channel] * weight + 32) >> 6;
                int is_log = channel < 3 ? partition_endpoints->rgb_is_log : partition_endpoints->alpha_is_log;
                texel[channel] = is_log ? log_value_to_float(value) : (float)value / 65535.0f;
            }
        }
    }
    return description->kind;
}

void astc_decode_image_hdr(const uint8_t *blocks, int image_width, int image_height, int block_width, int block_height, float *pixels_rgba)
{
    int columns = (image_width + block_width - 1) / block_width;
    int rows = (image_height + block_height - 1) / block_height;
    float block_texels[ASTC_MAX_BLOCK_TEXELS * 4];
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            astc_decode_block_hdr(blocks + (size_t)(row * columns + column) * ASTC_BLOCK_BYTES, block_width, block_height, block_texels);
            int origin_x = column * block_width;
            int origin_y = row * block_height;
            for (int y = 0; y < block_height && origin_y + y < image_height; y++) {
                int copy_width = block_width;
                if (origin_x + copy_width > image_width) {
                    copy_width = image_width - origin_x;
                }
                memcpy(pixels_rgba + ((size_t)(origin_y + y) * image_width + origin_x) * 4, block_texels + y * block_width * 4, (size_t)copy_width * 4 * sizeof(float));
            }
        }
    }
}

static uint8_t convert_unorm16_to_unorm8(int value)
{
    return (uint8_t)((value * 255 + 32767) / 65535);
}

void astc_decode_image_16bit(const uint8_t *blocks, int image_width, int image_height, int block_width, int block_height, int use_srgb, uint16_t *pixels_rgba16)
{
    int columns = (image_width + block_width - 1) / block_width;
    int rows = (image_height + block_height - 1) / block_height;
    uint16_t block_texels[ASTC_MAX_BLOCK_TEXELS * 4];
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            astc_decode_block(blocks + (size_t)(row * columns + column) * ASTC_BLOCK_BYTES, block_width, block_height, use_srgb, block_texels);
            int origin_x = column * block_width;
            int origin_y = row * block_height;
            for (int y = 0; y < block_height && origin_y + y < image_height; y++) {
                int copy_width = block_width;
                if (origin_x + copy_width > image_width) {
                    copy_width = image_width - origin_x;
                }
                memcpy(pixels_rgba16 + ((size_t)(origin_y + y) * image_width + origin_x) * 4, block_texels + y * block_width * 4, (size_t)copy_width * 4 * sizeof(uint16_t));
            }
        }
    }
}

void astc_decode_image(const uint8_t *blocks, int image_width, int image_height, int block_width, int block_height, int use_srgb, uint8_t *pixels_rgba8)
{
    int columns = (image_width + block_width - 1) / block_width;
    int rows = (image_height + block_height - 1) / block_height;
    uint16_t block_texels[ASTC_MAX_BLOCK_TEXELS * 4];
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            astc_decode_block(blocks + (size_t)(row * columns + column) * ASTC_BLOCK_BYTES, block_width, block_height, use_srgb, block_texels);
            int origin_x = column * block_width;
            int origin_y = row * block_height;
            for (int y = 0; y < block_height && origin_y + y < image_height; y++) {
                for (int x = 0; x < block_width && origin_x + x < image_width; x++) {
                    const uint16_t *texel = block_texels + (x + y * block_width) * 4;
                    uint8_t *pixel = pixels_rgba8 + ((size_t)(origin_y + y) * image_width + origin_x + x) * 4;
                    for (int channel = 0; channel < 3; channel++) {
                        pixel[channel] = use_srgb ? (uint8_t)(texel[channel] >> 8) : convert_unorm16_to_unorm8(texel[channel]);
                    }
                    pixel[3] = convert_unorm16_to_unorm8(texel[3]);
                }
            }
        }
    }
}

void astc_add_block_statistics(const uint8_t *blocks, size_t block_count, int block_width, int block_height, astc_block_statistics *statistics)
{
    for (size_t index = 0; index < block_count; index++) {
        astc_block_description description;
        astc_describe_block(blocks + index * ASTC_BLOCK_BYTES, block_width, block_height, &description);
        switch (description.kind) {
        case ASTC_BLOCK_NORMAL:
            statistics->normal_blocks++;
            statistics->partition_count_blocks[description.partition_count]++;
            for (int partition = 0; partition < description.partition_count; partition++) {
                statistics->endpoint_mode_partitions[description.endpoint_modes[partition]]++;
            }
            if (description.is_dual_plane) {
                statistics->dual_plane_blocks++;
            }
            if (description.uses_hdr_endpoint_mode) {
                statistics->hdr_endpoint_blocks++;
            }
            statistics->grid_size_blocks[description.grid_width][description.grid_height]++;
            break;
        case ASTC_BLOCK_VOID_EXTENT_LDR:
            statistics->void_extent_ldr_blocks++;
            break;
        case ASTC_BLOCK_VOID_EXTENT_HDR:
            statistics->void_extent_hdr_blocks++;
            break;
        default:
            statistics->illegal_blocks++;
            break;
        }
    }
}
