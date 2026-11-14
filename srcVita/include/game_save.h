#ifndef GAME_SAVE_H
#define GAME_SAVE_H

#include "asset_name.h"
#include "file.h"
#include "script_flags.h"
#include "script_machine.h"

#define GAME_SAVE_AUTO_PATH FILE_DATA_FOLDER "/save_auto.txt"
#define GAME_SAVE_MANUAL_PATH FILE_DATA_FOLDER "/save_manual.txt"
#define GAME_SAVE_TEXT_MAX 8192
#define GAME_SAVE_CHAPTER_COUNT 18
#define GAME_SAVE_CHAPTER_SELECT_SCRIPT "chapter"

enum game_save_slot {
    GAME_SAVE_AUTO,
    GAME_SAVE_MANUAL,
    GAME_SAVE_SLOT_COUNT
};

struct game_save {
    char script[ASSET_NAME_MAX];
    unsigned int serial;
    int chapter_mark;
    int phone_book_place;
    int fate_level;
    unsigned int pills_registered;
    int trick_is_on;
    int rollback_is_on;
    int has_soul_state;
    int game_cleared;
    unsigned char game_flags[SCRIPT_FLAG_BYTES];
    unsigned char chapter_flags[SCRIPT_FLAG_BYTES];
    unsigned char game_bank_saved[SCRIPT_GAME_FLAG_BANK_BYTES];
    unsigned char note_levels[SCRIPT_NOTE_LIST_COUNT][SCRIPT_NOTE_ID_COUNT];
};

void game_save_capture(struct game_save *save, const struct script_machine *machine, const char *script);
unsigned int game_save_write_text(const struct game_save *save, char *text, unsigned int text_size);
int game_save_read_text(struct game_save *save, const char *text, unsigned int text_size);
void game_save_apply_before_start(const struct game_save *save, struct script_machine *machine);
void game_save_apply_after_start(const struct game_save *save, struct script_machine *machine);
const char *game_save_path(enum game_save_slot slot);
int game_save_store(const struct game_save *save, enum game_save_slot slot);
int game_save_store_as_newest(struct game_save *save, enum game_save_slot slot);
int game_save_load(struct game_save *save, enum game_save_slot slot);
int game_save_load_newest(struct game_save *save);
int game_save_mark_cleared(void);
int game_save_erase_all(void);
int game_save_store_chapter(const struct game_save *save, int chapter);
int game_save_load_chapter(struct game_save *save, int chapter);
int game_save_for_chapter_select(struct game_save *save, int chapter);

#endif
