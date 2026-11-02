typedef struct Line1d95c {
    unsigned short speaker;
    unsigned short face;
    unsigned short icon;
    char pad6[0xe];
} Line1d95c;

typedef struct Text1d95c {
    int pad0;
    unsigned int line;
    char pad8[0xc];
    unsigned short speaker;
    unsigned short face;
    unsigned char mode;
    char pad19[0x1d5b];
    Line1d95c lines[1];
} Text1d95c;

typedef struct Speaker1d95c {
    unsigned short icon;
    char pad2[0xc];
} Speaker1d95c;

typedef struct Face1d95c {
    unsigned short icon;
    char pad2[4];
} Face1d95c;

extern unsigned short data_02084848[];
extern Speaker1d95c data_02085730[];
extern Face1d95c data_02099cb0[];
extern int func_02015d04(unsigned short c);

void func_0201d95c(Text1d95c *p, int unused, unsigned short *text)
{
    unsigned int lim[4];
    unsigned short speaker;
    unsigned short face;
    unsigned short c;

    face = 0xffff;
    lim[1] = face;
    lim[1] = lim[1] - 0xe4;
    lim[2] = face;
    lim[2] = lim[2] - 0xff;
    lim[3] = face;
    lim[3] = lim[3] - 0xff;
    lim[0] = face;
    lim[0] = lim[0] - 0xf7;
    speaker = 0;
    for (;;) {
        c = *text;
        if (c == lim[0]) {
            p->speaker = text[1];
            break;
        }
        if (c == 0xff18 || c == 0xff19) {
            p->speaker = text[1];
            break;
        }
        if (c == lim[1]) {
            p->face = text[1];
            break;
        }
        if (c < lim[2] || func_02015d04(c) != 0) {
            if (p->mode == 4) {
                face = p->face;
            } else {
                speaker = p->speaker;
            }
            break;
        }
        text = text + data_02084848[(unsigned short)(*text - lim[3])] + 1;
    }
    p->lines[p->line].speaker = speaker;
    p->lines[p->line].face = face;
    if (p->mode == 0) {
        if (p->speaker != 0) {
            p->lines[p->line].icon = data_02085730[p->speaker].icon;
        }
    } else if (p->mode == 4) {
        if (p->face != 0xffff) {
            p->lines[p->line].icon = data_02099cb0[p->face].icon;
        }
    }
    if (p->mode == 0) {
        if (speaker != 0) {
            p->lines[p->line].icon = data_02085730[speaker].icon;
        }
    } else if (p->mode == 4) {
        if (face != 0xffff) {
            p->lines[p->line].icon = data_02099cb0[face].icon;
        }
    }
}
