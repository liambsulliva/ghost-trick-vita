#include "packed_file.h"

#include <string.h>

#define PACKED_FILE_MAGIC "GPK1"
#define PACKED_FILE_MAGIC_SIZE 4u
#define LONGEST_CODE_BITS 15
#define LITERAL_LENGTH_CODES 288
#define DISTANCE_CODES 30
#define CODE_LENGTH_CODES 19
#define FAST_CODE_BITS 9
#define FAST_TABLE_SIZE (1u << FAST_CODE_BITS)
#define FAST_LENGTH_SHIFT 9
#define FAST_SYMBOL_MASK 0x1ffu
#define END_OF_BLOCK 256
#define FIRST_LENGTH_SYMBOL 257
#define LENGTH_SYMBOLS 29
#define STORED_BLOCK 0
#define FIXED_BLOCK 1
#define DYNAMIC_BLOCK 2
#define REPEAT_PREVIOUS_LENGTH 16
#define REPEAT_ZERO_SHORT 17
#define REPEAT_ZERO_LONG 18
#define MOST_LITERAL_LENGTH_CODES_SENT 286
#define FIXED_DISTANCE_CODE_BITS 5

struct bit_reader {
    const unsigned char *input;
    unsigned int input_size;
    unsigned int position;
    unsigned int bits;
    unsigned int bit_count;
    int ran_out;
};

struct huffman {
    unsigned short counts[LONGEST_CODE_BITS + 1];
    unsigned short symbols[LITERAL_LENGTH_CODES];
    unsigned short fast[FAST_TABLE_SIZE];
};

struct inflater {
    struct bit_reader reader;
    unsigned char *output;
    unsigned int output_size;
    unsigned int written;
};

static const unsigned short length_bases[LENGTH_SYMBOLS] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
static const unsigned char length_extra_bits[LENGTH_SYMBOLS] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
static const unsigned short distance_bases[DISTANCE_CODES] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };
static const unsigned char distance_extra_bits[DISTANCE_CODES] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };
static const unsigned char code_length_order[CODE_LENGTH_CODES] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

static unsigned int read_u32_little_endian(const unsigned char *bytes)
{
    return (unsigned int)bytes[0] | ((unsigned int)bytes[1] << 8) | ((unsigned int)bytes[2] << 16) | ((unsigned int)bytes[3] << 24);
}

int packed_file_is_packed(const unsigned char *bytes, unsigned int size)
{
    return size >= PACKED_FILE_HEADER_SIZE && memcmp(bytes, PACKED_FILE_MAGIC, PACKED_FILE_MAGIC_SIZE) == 0;
}

unsigned int packed_file_unpacked_size(const unsigned char *bytes)
{
    return read_u32_little_endian(&bytes[PACKED_FILE_MAGIC_SIZE]);
}

static void fill_bits(struct bit_reader *reader)
{
    while (reader->bit_count <= 24u && reader->position < reader->input_size) {
        reader->bits = reader->bits | ((unsigned int)reader->input[reader->position] << reader->bit_count);
        reader->position = reader->position + 1u;
        reader->bit_count = reader->bit_count + 8u;
    }
}

static unsigned int take_bits(struct bit_reader *reader, unsigned int count)
{
    unsigned int value;

    if (count == 0u) {
        return 0u;
    }
    fill_bits(reader);
    if (reader->bit_count < count) {
        reader->ran_out = 1;
        return 0u;
    }
    value = reader->bits & ((1u << count) - 1u);
    reader->bits = reader->bits >> count;
    reader->bit_count = reader->bit_count - count;
    return value;
}

static unsigned int reversed_bits(unsigned int code, unsigned int length)
{
    unsigned int reversed;
    unsigned int index;

    reversed = 0u;
    for (index = 0u; index < length; index++) {
        reversed = (reversed << 1) | ((code >> index) & 1u);
    }
    return reversed;
}

static int build_huffman(struct huffman *huffman, const unsigned char *lengths, int symbol_count)
{
    unsigned short offsets[LONGEST_CODE_BITS + 1];
    unsigned int next_code[LONGEST_CODE_BITS + 1];
    unsigned int code;
    int left;
    int length;
    int symbol;

    memset(huffman->counts, 0, sizeof(huffman->counts));
    memset(huffman->fast, 0, sizeof(huffman->fast));
    for (symbol = 0; symbol < symbol_count; symbol++) {
        huffman->counts[lengths[symbol]] = (unsigned short)(huffman->counts[lengths[symbol]] + 1u);
    }
    huffman->counts[0] = 0;
    left = 1;
    for (length = 1; length <= LONGEST_CODE_BITS; length++) {
        left = (left << 1) - huffman->counts[length];
        if (left < 0) {
            return 0;
        }
    }
    offsets[1] = 0;
    for (length = 1; length < LONGEST_CODE_BITS; length++) {
        offsets[length + 1] = (unsigned short)(offsets[length] + huffman->counts[length]);
    }
    for (symbol = 0; symbol < symbol_count; symbol++) {
        if (lengths[symbol] != 0) {
            huffman->symbols[offsets[lengths[symbol]]] = (unsigned short)symbol;
            offsets[lengths[symbol]] = (unsigned short)(offsets[lengths[symbol]] + 1u);
        }
    }
    code = 0u;
    next_code[0] = 0u;
    for (length = 1; length <= LONGEST_CODE_BITS; length++) {
        code = (code + huffman->counts[length - 1]) << 1;
        next_code[length] = code;
    }
    for (symbol = 0; symbol < symbol_count; symbol++) {
        unsigned int symbol_length;
        unsigned int slot;

        symbol_length = lengths[symbol];
        if (symbol_length == 0u) {
            continue;
        }
        code = next_code[symbol_length];
        next_code[symbol_length] = code + 1u;
        if (symbol_length > FAST_CODE_BITS) {
            continue;
        }
        for (slot = reversed_bits(code, symbol_length); slot < FAST_TABLE_SIZE; slot = slot + (1u << symbol_length)) {
            huffman->fast[slot] = (unsigned short)((symbol_length << FAST_LENGTH_SHIFT) | (unsigned int)symbol);
        }
    }
    return 1;
}

static int decode_slowly(struct bit_reader *reader, const struct huffman *huffman)
{
    int code;
    int first;
    int index;
    int length;

    code = 0;
    first = 0;
    index = 0;
    for (length = 1; length <= LONGEST_CODE_BITS; length++) {
        int count;

        code = code | (int)take_bits(reader, 1u);
        if (reader->ran_out) {
            return -1;
        }
        count = huffman->counts[length];
        if (code - first < count) {
            return huffman->symbols[index + (code - first)];
        }
        index = index + count;
        first = (first + count) << 1;
        code = code << 1;
    }
    return -1;
}

static int decode_symbol(struct bit_reader *reader, const struct huffman *huffman)
{
    unsigned int entry;
    unsigned int length;

    fill_bits(reader);
    entry = huffman->fast[reader->bits & (FAST_TABLE_SIZE - 1u)];
    if (entry == 0u) {
        return decode_slowly(reader, huffman);
    }
    length = entry >> FAST_LENGTH_SHIFT;
    if (reader->bit_count < length) {
        reader->ran_out = 1;
        return -1;
    }
    reader->bits = reader->bits >> length;
    reader->bit_count = reader->bit_count - length;
    return (int)(entry & FAST_SYMBOL_MASK);
}

static int copy_match(struct inflater *inflater, unsigned int length, unsigned int distance)
{
    unsigned char *destination;
    const unsigned char *source;
    unsigned int index;

    if (distance == 0u || distance > inflater->written || length > inflater->output_size - inflater->written) {
        return 0;
    }
    destination = &inflater->output[inflater->written];
    source = destination - distance;
    if (distance >= length) {
        memcpy(destination, source, length);
    } else {
        for (index = 0u; index < length; index++) {
            destination[index] = source[index];
        }
    }
    inflater->written = inflater->written + length;
    return 1;
}

static int inflate_codes(struct inflater *inflater, const struct huffman *literal_lengths, const struct huffman *distances)
{
    struct bit_reader *reader;

    reader = &inflater->reader;
    for (;;) {
        int symbol;
        unsigned int length;
        int distance_symbol;
        unsigned int distance;

        symbol = decode_symbol(reader, literal_lengths);
        if (symbol < 0) {
            return 0;
        }
        if (symbol < END_OF_BLOCK) {
            if (inflater->written >= inflater->output_size) {
                return 0;
            }
            inflater->output[inflater->written] = (unsigned char)symbol;
            inflater->written = inflater->written + 1u;
            continue;
        }
        if (symbol == END_OF_BLOCK) {
            return 1;
        }
        symbol = symbol - FIRST_LENGTH_SYMBOL;
        if (symbol >= LENGTH_SYMBOLS) {
            return 0;
        }
        length = length_bases[symbol] + take_bits(reader, length_extra_bits[symbol]);
        distance_symbol = decode_symbol(reader, distances);
        if (distance_symbol < 0 || distance_symbol >= DISTANCE_CODES) {
            return 0;
        }
        distance = distance_bases[distance_symbol] + take_bits(reader, distance_extra_bits[distance_symbol]);
        if (reader->ran_out || !copy_match(inflater, length, distance)) {
            return 0;
        }
    }
}

static int inflate_stored(struct inflater *inflater)
{
    struct bit_reader *reader;
    unsigned int length;
    unsigned int complement;
    unsigned int index;

    reader = &inflater->reader;
    (void)take_bits(reader, reader->bit_count & 7u);
    length = take_bits(reader, 16u);
    complement = take_bits(reader, 16u);
    if (reader->ran_out || length != (~complement & 0xffffu) || length > inflater->output_size - inflater->written) {
        return 0;
    }
    for (index = 0u; index < length; index++) {
        inflater->output[inflater->written] = (unsigned char)take_bits(reader, 8u);
        inflater->written = inflater->written + 1u;
    }
    return !reader->ran_out;
}

static int inflate_fixed(struct inflater *inflater)
{
    static struct huffman literal_lengths;
    static struct huffman distances;
    static int are_built;

    if (!are_built) {
        unsigned char lengths[LITERAL_LENGTH_CODES];
        int symbol;

        for (symbol = 0; symbol < LITERAL_LENGTH_CODES; symbol++) {
            lengths[symbol] = (unsigned char)(symbol < 144 ? 8 : symbol < 256 ? 9 : symbol < 280 ? 7 : 8);
        }
        (void)build_huffman(&literal_lengths, lengths, LITERAL_LENGTH_CODES);
        for (symbol = 0; symbol < DISTANCE_CODES; symbol++) {
            lengths[symbol] = FIXED_DISTANCE_CODE_BITS;
        }
        (void)build_huffman(&distances, lengths, DISTANCE_CODES);
        are_built = 1;
    }
    return inflate_codes(inflater, &literal_lengths, &distances);
}

static int read_code_lengths(struct bit_reader *reader, const struct huffman *code_lengths, unsigned char *lengths, int total)
{
    int index;

    index = 0;
    while (index < total) {
        int symbol;
        unsigned int repeat;
        unsigned char value;

        symbol = decode_symbol(reader, code_lengths);
        if (symbol < 0) {
            return 0;
        }
        if (symbol < REPEAT_PREVIOUS_LENGTH) {
            lengths[index] = (unsigned char)symbol;
            index = index + 1;
            continue;
        }
        value = 0;
        if (symbol == REPEAT_PREVIOUS_LENGTH) {
            if (index == 0) {
                return 0;
            }
            value = lengths[index - 1];
            repeat = 3u + take_bits(reader, 2u);
        } else if (symbol == REPEAT_ZERO_SHORT) {
            repeat = 3u + take_bits(reader, 3u);
        } else {
            repeat = 11u + take_bits(reader, 7u);
        }
        if (reader->ran_out || index + (int)repeat > total) {
            return 0;
        }
        while (repeat > 0u) {
            lengths[index] = value;
            index = index + 1;
            repeat = repeat - 1u;
        }
    }
    return 1;
}

static int inflate_dynamic(struct inflater *inflater)
{
    struct bit_reader *reader;
    struct huffman code_lengths;
    struct huffman literal_lengths;
    struct huffman distances;
    unsigned char lengths[LITERAL_LENGTH_CODES + DISTANCE_CODES];
    unsigned char code_length_lengths[CODE_LENGTH_CODES];
    int literal_length_count;
    int distance_count;
    int code_length_count;
    int index;

    reader = &inflater->reader;
    literal_length_count = (int)take_bits(reader, 5u) + 257;
    distance_count = (int)take_bits(reader, 5u) + 1;
    code_length_count = (int)take_bits(reader, 4u) + 4;
    if (reader->ran_out || literal_length_count > MOST_LITERAL_LENGTH_CODES_SENT || distance_count > DISTANCE_CODES) {
        return 0;
    }
    memset(code_length_lengths, 0, sizeof(code_length_lengths));
    for (index = 0; index < code_length_count; index++) {
        code_length_lengths[code_length_order[index]] = (unsigned char)take_bits(reader, 3u);
    }
    if (reader->ran_out || !build_huffman(&code_lengths, code_length_lengths, CODE_LENGTH_CODES)) {
        return 0;
    }
    if (!read_code_lengths(reader, &code_lengths, lengths, literal_length_count + distance_count) || lengths[END_OF_BLOCK] == 0) {
        return 0;
    }
    if (!build_huffman(&literal_lengths, lengths, literal_length_count) || !build_huffman(&distances, lengths + literal_length_count, distance_count)) {
        return 0;
    }
    return inflate_codes(inflater, &literal_lengths, &distances);
}

int packed_file_inflate(const unsigned char *input, unsigned int input_size, unsigned char *output, unsigned int output_size)
{
    struct inflater inflater;
    unsigned int is_last;

    memset(&inflater, 0, sizeof(inflater));
    inflater.reader.input = input;
    inflater.reader.input_size = input_size;
    inflater.output = output;
    inflater.output_size = output_size;
    do {
        unsigned int block_type;
        int is_whole;

        is_last = take_bits(&inflater.reader, 1u);
        block_type = take_bits(&inflater.reader, 2u);
        if (inflater.reader.ran_out) {
            return 0;
        }
        if (block_type == STORED_BLOCK) {
            is_whole = inflate_stored(&inflater);
        } else if (block_type == FIXED_BLOCK) {
            is_whole = inflate_fixed(&inflater);
        } else if (block_type == DYNAMIC_BLOCK) {
            is_whole = inflate_dynamic(&inflater);
        } else {
            is_whole = 0;
        }
        if (!is_whole) {
            return 0;
        }
    } while (!is_last);
    return inflater.written == output_size;
}

int packed_file_unpack(const unsigned char *bytes, unsigned int size, unsigned char *unpacked, unsigned int unpacked_size)
{
    if (!packed_file_is_packed(bytes, size) || unpacked_size != packed_file_unpacked_size(bytes)) {
        return 0;
    }
    return packed_file_inflate(bytes + PACKED_FILE_HEADER_SIZE, size - PACKED_FILE_HEADER_SIZE, unpacked, unpacked_size);
}
