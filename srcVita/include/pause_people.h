#ifndef PAUSE_PEOPLE_H
#define PAUSE_PEOPLE_H

#define PAUSE_PEOPLE_STAYS 0
#define PAUSE_PEOPLE_LEAVES 1
#define PAUSE_PEOPLE_TAB_PEOPLE 0
#define PAUSE_PEOPLE_TAB_PLACES 1
#define PAUSE_PEOPLE_SLOTS 7

int pause_people_read_text(const char *text, unsigned int text_size);
int pause_people_load(const char *assets_folder);
int pause_people_is_loaded(void);
void pause_people_use_notes(const unsigned char *levels);
const unsigned char *pause_people_every_note(void);
void pause_people_enter(void);
int pause_people_step(unsigned int pressed);
unsigned int pause_people_touch(int lands, float x, float y);
int pause_people_tab(void);
int pause_people_selected(void);
int pause_people_first_slot(void);
int pause_people_count(int tab);
int pause_people_entry(int tab, int index);
int pause_people_level(int tab, int index);
const char *pause_people_card(void);
const char *pause_people_art(void);
void pause_people_draw(float x_offset);
void pause_people_draw_bar(float x_offset);

#endif
