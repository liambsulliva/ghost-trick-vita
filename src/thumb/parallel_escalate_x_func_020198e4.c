typedef struct Text198e4 {
    char pad0[0x14];
    unsigned short *text;
    char pad18[0x1c];
    unsigned short width;
    short x;
    char pad38[2];
    unsigned char lines;
    unsigned char column;
    unsigned short height;
    char pad3e[2];
    short indent;
    char pad42[0xc];
    unsigned char pending;
    unsigned char pages;
    char pad50;
    unsigned char margin;
    char pad52[2];
    int state;
    char pad58[2];
    unsigned char wait;
    unsigned char skip;
    char pad5c[2];
    unsigned short prev;
} Text198e4;

extern int func_02017110(Text198e4 *t, unsigned short *text);

int func_020198e4(Text198e4 *t)
{
    t->prev = 0xfffe;
    t->lines++;
    t->column = 0;
    t->width = 0;
    t->x += (short)(t->margin + t->indent);
    t->wait = 0;
    t->skip = 0;
    t->height = 0;
    if (t->pending != 0) {
        t->pending = 0;
        t->state = 0;
    }
    if (t->state == 1) {
        t->pages++;
    }
    func_02017110(t, t->text + 1);
    return 0;
}
