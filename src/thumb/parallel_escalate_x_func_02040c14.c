struct TimeText {
    unsigned char pad[0x54];
    signed char tens;
    signed char ones;
    unsigned char pad56[3];
    unsigned char morning;
};

void func_02040c14(struct TimeText *clock, signed char *text)
{
    signed char hour;
    int morning;

    morning = 1;
    hour = (clock->tens - 48) * 10 + (clock->ones - 48);
    if (hour > 24) {
        hour = hour % 24;
    }
    if (hour > 11 && hour < 24) {
        morning = 0;
    }
    if (hour > 12) {
        hour -= 12;
    }
    if (hour == 0) {
        hour += 12;
    }
    clock->morning = morning;
    text[0] = 48;
    text[0] += (signed char)(hour / 10);
    text[1] = (hour % 10) + 48;
}
