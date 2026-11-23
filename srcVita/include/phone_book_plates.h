#ifndef PHONE_BOOK_PLATES_H
#define PHONE_BOOK_PLATES_H

#define PHONE_BOOK_PLATES_PLACE_MOST 24
#define PHONE_BOOK_PLATES_LEVELS 3
#define PHONE_BOOK_PLATES_CHOICES 2

struct phone_book_plate {
    int resource;
    float x;
    float y;
    float width;
    float height;
    float left;
    float top;
};

int phone_book_plates_read_text(const char *text, unsigned int text_size);
int phone_book_plates_load(const char *assets_folder);
int phone_book_plates_is_loaded(void);
int phone_book_plates_level(int place, int stored_note_level);
const struct phone_book_plate *phone_book_plates_find(int place, int stored_note_level, int is_selected);
void phone_book_plates_draw(int place, int stored_note_level, int is_selected, float x, float y, float scale);
const struct phone_book_plate *phone_book_plates_find_choice(int kind, int is_selected);
const struct phone_book_plate *phone_book_plates_find_prompt(const char *name);
void phone_book_plates_draw_choice(int kind, int is_selected, float x, float y, float scale);
void phone_book_plates_draw_prompt(const char *name, float x, float y, float scale);

#endif
