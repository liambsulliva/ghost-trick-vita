#ifndef SCRIPT_TALK_H
#define SCRIPT_TALK_H

#include "asset_name.h"
#include "script_narration.h"

#define SCRIPT_TALK_MESSAGE_MAX 1024
#define SCRIPT_TALK_PAGE_MAX 4096
#define SCRIPT_TALK_LINE_MAX 8192
#define SCRIPT_TALK_TEXT_MAX 200000
#define SCRIPT_TALK_GLYPH_MAX 512
#define SCRIPT_TALK_COLOR_COUNT 4
#define SCRIPT_TALK_COLOR_SHIFT 14
#define SCRIPT_TALK_GLYPH_ID_MASK 0x3fffu

enum script_talk_ending {
    SCRIPT_TALK_ENDS_ON_PRESS,
    SCRIPT_TALK_ENDS_ON_FORWARD_THEN_NEXT,
    SCRIPT_TALK_ENDS_ON_FORWARD_THEN_CLOSE
};

enum script_talk_phase {
    SCRIPT_TALK_CLOSED,
    SCRIPT_TALK_OPENING,
    SCRIPT_TALK_REVEALING,
    SCRIPT_TALK_WAITING,
    SCRIPT_TALK_SWITCHING_OUT,
    SCRIPT_TALK_CLOSING
};

struct script_talk_glyph {
    short atlas_x;
    short atlas_y;
    short width;
    short height;
    short left;
    short top;
    short advance;
};

struct script_talk_line {
    short x;
    short y;
    unsigned short glyph_count;
    int first_text;
};

struct script_talk_page {
    char face_texture[ASSET_NAME_MAX];
    int has_face;
    int face_is_flipped;
    int is_top_box;
    int reveal_is_fade;
    enum script_talk_ending ending;
    int first_line;
    int line_count;
    int glyph_count;
};

struct script_talk_message {
    char label[SCRIPT_CUE_LABEL_MAX];
    int first_page;
    int page_count;
};

struct script_talk_book {
    char font_texture[ASSET_NAME_MAX];
    struct script_talk_glyph glyphs[SCRIPT_TALK_GLYPH_MAX];
    struct script_talk_message messages[SCRIPT_TALK_MESSAGE_MAX];
    struct script_talk_page pages[SCRIPT_TALK_PAGE_MAX];
    struct script_talk_line lines[SCRIPT_TALK_LINE_MAX];
    unsigned short text[SCRIPT_TALK_TEXT_MAX];
    int glyph_count;
    int message_count;
    int page_count;
    int line_count;
    int text_count;
};

struct script_talk {
    enum script_talk_phase phase;
    const struct script_talk_message *message;
    int page_index;
    int timer;
    int glyphs_shown;
    int fade_ticks;
    int press_is_pending;
    int forward_is_pending;
    int close_is_scheduled;
};

struct script_talk_view {
    const struct script_talk_book *book;
    const struct script_talk_page *page;
    float slide;
    float text_alpha;
    int glyphs_shown;
    int cursor_is_on;
};

int script_talk_book_parse(const char *text, unsigned int text_size, struct script_talk_book *book);
const struct script_talk_message *script_talk_book_find(const struct script_talk_book *book, const char *label);

#endif
