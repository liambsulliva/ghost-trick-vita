typedef struct Font1cac8 Font1cac8;

typedef struct FontVt1cac8 {
    void *slots[7];
    int (*width)(Font1cac8 *font, unsigned short code);
} FontVt1cac8;

struct Font1cac8 {
    FontVt1cac8 *vt;
};

typedef struct Size1cac8 {
    unsigned short code;
    unsigned short width;
} Size1cac8;

typedef struct Pair1cac8 {
    unsigned char a;
    unsigned char b;
} Pair1cac8;

typedef struct Text1cac8 {
    Font1cac8 *font;
    int pad4;
    unsigned char *dst;
    unsigned char *glyph;
    unsigned short pitch;
    unsigned short rows;
    int pad14;
    unsigned short code;
    char pad1a[2];
    unsigned char scale;
    char pad1d;
    short cursor;
    char pad20[2];
    unsigned char advance;
    unsigned char line;
    unsigned char glyph_w;
    unsigned char glyph_h;
    unsigned short base;
    short left;
    short top;
    unsigned short right;
    unsigned short bottom;
    char pad30[4];
    unsigned char sizes;
    unsigned char wide;
    char pad36[2];
    void *ruby;
    char pad3c;
    unsigned char gap;
    unsigned short style;
    char pad40[0x1cc4];
    Size1cac8 size[1];
} Text1cac8;

extern void func_0207c594(void *dst, int size);
extern Pair1cac8 func_0200e410(void *ruby, unsigned short code, unsigned short style);
extern void func_0201cabc(Pair1cac8 *out, Pair1cac8 *in);
extern unsigned char func_0201cf7c(Text1cac8 *t, unsigned short code, unsigned short style);
extern int func_0201cff0(Text1cac8 *t, unsigned short code, unsigned short style);
extern int func_0201cfd0(Text1cac8 *t, unsigned short code);
extern int func_0201ce60(Text1cac8 *t, unsigned short x, unsigned short limit);
extern void func_0201ce54(Text1cac8 *t, unsigned char *dst, unsigned short pitch, unsigned char color);
extern int func_0201cda8(Text1cac8 *t, short row);
extern int func_0201cdbc(Text1cac8 *t, int x);

int func_0201cac8(Text1cac8 *t)
{
    unsigned char hi;
    signed char shift;
    unsigned char *dst;
    Pair1cac8 buf;
    short x;
    unsigned short adv;
    unsigned short override;
    short y;
    unsigned char b;
    short row0;
    int width;
    unsigned char n;
    Pair1cac8 pair;
    short row;
    unsigned char lo;
    unsigned int i;
    unsigned int j;
    unsigned int k;
    unsigned short w;
    unsigned short right;
    short xw;
    short xn;

    dst = t->dst;
    x = t->cursor + t->left;
    y = t->top + (t->line + t->base) * t->scale;
    override = 0;
    b = t->sizes;
    if (b != 0) {
        for (i = 0; i < b; i++) {
            if (t->code == t->size[i].code) {
                override = t->size[i].width;
                break;
            }
        }
    }
    adv = t->advance;
    func_0207c594(&buf, 2);
    if (t->ruby != 0) {
        pair = func_0200e410(t->ruby, t->code, t->style);
        func_0201cabc(&buf, &pair);
        adv = func_0201cf7c(t, t->code, t->style);
    }
    if (func_0201cff0(t, t->code, t->style) != 0) {
        x = x + t->gap;
        adv = adv + t->gap;
    }
    w = adv;
    if (override != 0) {
        w = override;
    }
    t->cursor += (short)w;
    shift = 0;
    width = t->font->vt->width(t->font, t->code);
    if (buf.a != 0) {
        shift = -(signed char)buf.a;
        width = (signed char)(width + (signed char)func_0201cfd0(t, t->code));
    }
    if (x + t->glyph_w > t->right && func_0201ce60(t, x + shift, t->right) != 0) {
        return 2;
    }
    if (y + t->glyph_h > t->bottom) {
        return 1;
    }
    if (t->wide) {
        if (override != 0) {
            adv = override;
        }
        t->cursor += (short)adv;
        row0 = y;
        row0 -= 0xe;
        for (j = 0; j < t->glyph_h; j++) {
            row = row0 + j * 2;
            if (row < 0 || row >= t->rows - 1) {
                continue;
            }
            for (k = 0; k < t->glyph_w / 2; k++) {
                xw = x + k * 4;
                if (xw < 0 || xw >= t->pitch - 1) {
                    continue;
                }
                b = t->glyph[(unsigned short)(k + j * t->glyph_w / 2)];
                hi = b & 0xf0;
                lo = b & 0xf;
                if (lo) {
                    func_0201ce54(t, dst + row * t->pitch + xw, t->pitch, lo);
                }
                if (hi) {
                    func_0201ce54(t, dst + row * t->pitch + xw + 2, t->pitch, (unsigned char)(hi >> 4));
                }
            }
        }
    } else {
        for (j = 0; j < t->glyph_h; j++) {
            row = y + j + width;
            if (func_0201cda8(t, row) != 0) {
                continue;
            }
            for (k = 0; k < t->glyph_w / 2; k++) {
                xn = x + k * 2 + shift;
                b = t->glyph[(unsigned short)(k + j * t->glyph_w / 2)];
                lo = b & 0xf0;
                hi = b & 0xf;
                if (hi && func_0201cdbc(t, xn) == 0) {
                    dst[row * t->pitch + xn] = hi;
                }
                if (lo && func_0201cdbc(t, xn + 1) == 0) {
                    dst[row * t->pitch + xn + 1] = lo >> 4;
                }
            }
        }
    }
    return 0;
}
