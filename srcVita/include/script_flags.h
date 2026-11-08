#ifndef SCRIPT_FLAGS_H
#define SCRIPT_FLAGS_H

#define SCRIPT_FLAG_COUNT 4096
#define SCRIPT_FLAG_BYTES (SCRIPT_FLAG_COUNT / 8)
#define SCRIPT_GAME_FLAG_BANK_BYTES 128

struct script_flags {
    unsigned char local[SCRIPT_FLAG_BYTES];
    unsigned char game[SCRIPT_FLAG_BYTES];
    unsigned char chapter[SCRIPT_FLAG_BYTES];
    unsigned char game_bank_saved[SCRIPT_GAME_FLAG_BANK_BYTES];
};

void script_flags_clear_all(struct script_flags *flags);
void script_flags_clear_local(struct script_flags *flags);
int script_flags_is_known(unsigned int flag);
int script_flags_read(const struct script_flags *flags, unsigned int flag);
void script_flags_write(struct script_flags *flags, unsigned int flag, int value);
int script_flags_read_known(const struct script_flags *flags, unsigned int flag, int *value);
int script_flags_write_known(struct script_flags *flags, unsigned int flag, int value);
int script_flags_write_byte(struct script_flags *flags, unsigned int first_flag, unsigned int bits);
int script_flags_read_byte(const struct script_flags *flags, unsigned int first_flag);
void script_flags_save_game_bank(struct script_flags *flags);
void script_flags_restore_game_bank(struct script_flags *flags);

#endif
