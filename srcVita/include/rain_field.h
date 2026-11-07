#ifndef RAIN_FIELD_H
#define RAIN_FIELD_H

#define RAIN_FIELD_STREAK_MAX 137

struct rain_streak {
    float x;
    float y;
    float speed;
    float half_length;
    int is_falling;
};

struct rain_field {
    struct rain_streak streaks[RAIN_FIELD_STREAK_MAX];
    int count;
    int span;
    int first_x;
    unsigned int random_state;
};

void rain_field_seed(struct rain_field *field);
void rain_field_start(struct rain_field *field, int span, int first_x, float center_x, float top_y);
void rain_field_advance(struct rain_field *field, float center_x, float top_y);
float rain_field_alpha_of(int index);

#endif
