#include <string.h>
#include "script_flags.h"

#define CHAPTER_FLAG_BIT 0x8000u
#define GAME_FLAG_BIT 0x1000u
#define FLAG_THAT_ALWAYS_READS_ZERO 0x1000u
#define FLAGS_IN_A_FLAG_BYTE 8u
#define FLAG_INDEX_MASK (SCRIPT_FLAG_COUNT - 1u)

static unsigned int index_of(unsigned int flag)
{
    return flag & ~(CHAPTER_FLAG_BIT | GAME_FLAG_BIT);
}

void script_flags_clear_all(struct script_flags *flags)
{
    memset(flags, 0, sizeof(*flags));
}

void script_flags_clear_local(struct script_flags *flags)
{
    memset(flags->local, 0, sizeof(flags->local));
}

void script_flags_clear_chapter(struct script_flags *flags)
{
    memset(flags->chapter, 0, sizeof(flags->chapter));
}

int script_flags_is_known(unsigned int flag)
{
    return index_of(flag) <= FLAG_INDEX_MASK;
}

static const unsigned char *store_to_read(const struct script_flags *flags, unsigned int flag)
{
    if (flag & CHAPTER_FLAG_BIT) {
        return flags->chapter;
    }
    if (flag & GAME_FLAG_BIT) {
        return flags->game;
    }
    return flags->local;
}

static unsigned char *store_to_write(struct script_flags *flags, unsigned int flag)
{
    if (flag & CHAPTER_FLAG_BIT) {
        return flags->chapter;
    }
    if (flag & GAME_FLAG_BIT) {
        return flags->game;
    }
    return flags->local;
}

int script_flags_read(const struct script_flags *flags, unsigned int flag)
{
    unsigned int index;

    if (flag == FLAG_THAT_ALWAYS_READS_ZERO) {
        return 0;
    }
    index = index_of(flag);
    return (store_to_read(flags, flag)[index / 8] >> (index % 8)) & 1;
}

void script_flags_save_game_bank(struct script_flags *flags)
{
    memcpy(flags->game_bank_saved, flags->game, sizeof(flags->game_bank_saved));
}

void script_flags_restore_game_bank(struct script_flags *flags)
{
    memcpy(flags->game, flags->game_bank_saved, sizeof(flags->game_bank_saved));
}

void script_flags_write(struct script_flags *flags, unsigned int flag, int value)
{
    unsigned char *store;
    unsigned int index;

    store = store_to_write(flags, flag);
    index = index_of(flag);
    if (value) {
        store[index / 8] = (unsigned char)(store[index / 8] | (1u << (index % 8)));
    } else {
        store[index / 8] = (unsigned char)(store[index / 8] & ~(1u << (index % 8)));
    }
}

int script_flags_read_known(const struct script_flags *flags, unsigned int flag, int *value)
{
    if (!script_flags_is_known(flag)) {
        return 0;
    }
    *value = script_flags_read(flags, flag);
    return 1;
}

int script_flags_write_known(struct script_flags *flags, unsigned int flag, int value)
{
    if (!script_flags_is_known(flag)) {
        return 0;
    }
    script_flags_write(flags, flag, value);
    return 1;
}

int script_flags_write_byte(struct script_flags *flags, unsigned int first_flag, unsigned int bits)
{
    unsigned int index;

    for (index = 0; index < FLAGS_IN_A_FLAG_BYTE; index++) {
        if (!script_flags_write_known(flags, first_flag + index, (int)((bits >> index) & 1u))) {
            return 0;
        }
    }
    return 1;
}

int script_flags_read_byte(const struct script_flags *flags, unsigned int first_flag)
{
    unsigned int index;
    int mask;
    int value;

    mask = 0;
    for (index = 0; index < FLAGS_IN_A_FLAG_BYTE; index++) {
        if (script_flags_read_known(flags, first_flag + index, &value) && value) {
            mask = mask | (1 << index);
        }
    }
    return mask;
}
