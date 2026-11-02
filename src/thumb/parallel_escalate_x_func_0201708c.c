typedef struct Text1708c {
    char pad0[0x14];
    unsigned short *text;
    char pad18[0x1c];
    unsigned short width;
    short x;
    char pad38[2];
    unsigned char lines;
    unsigned char column;
    unsigned short height;
    unsigned short depth;
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
} Text1708c;

int func_0201708c(Text1708c *t)
{
    t->prev = 0xfffe;
    t->lines++;
    t->column = 0;
    t->width = 0;
    t->x = t->margin + (t->margin + t->indent) * t->lines;
    t->height = 0;
    t->wait = 0;
    t->skip = 0;
    t->depth = 0;
    if (t->pending != 0) {
        t->pending = 0;
        t->state = 0;
    }
    if (t->state == 1) {
        t->pages++;
    }
    return 0;
}
