struct Field;
struct Obj;

struct ObjVtbl {
    char pad0[0x1c];
    signed char (*get)(struct Obj *obj, unsigned int id);
};

struct Obj {
    struct ObjVtbl *vtbl;
};

struct Entry {
    unsigned short id;
    unsigned short width;
};

struct Board {
    struct Obj *obj;
    int pad4;
    unsigned char *base;
    unsigned char *data;
    unsigned short width;
    unsigned short height;
    char pad14[4];
    unsigned short id;
    char pad1a[2];
    unsigned char scale;
    char pad1d;
    short cx;
    char pad20[2];
    unsigned char adv;
    unsigned char line;
    unsigned char cell_w;
    unsigned char cell_h;
    unsigned short gap;
    short ox;
    short oy;
    unsigned short max_x;
    unsigned short max_y;
    unsigned short used_x;
    unsigned short used_y;
    unsigned char count;
    unsigned char big;
    char pad36[2];
    struct Field *field;
    char pad3c;
    unsigned char extra;
    unsigned short sub;
    char pad40[0x1cc4];
    struct Entry table[1];
};

struct Code {
    unsigned char lo;
    unsigned char hi;
};

extern void func_0207c594(void *object, int size);
extern struct Code func_0200e410(struct Field *field, unsigned short id, unsigned short mask);
extern void func_0201cabc(struct Code *last, struct Code *code);
extern unsigned char func_0201cf7c(struct Board *board, unsigned int id, unsigned int sub);
extern int func_0201cff0(struct Board *board, unsigned int a, unsigned int b);
extern int func_0201cfd0(struct Board *board, unsigned int id);
extern int func_0201ce60(struct Board *board, unsigned short x, unsigned short limit);
extern void func_0201ce3c(struct Board *board, unsigned char *dst, unsigned int pitch, unsigned int value);
extern void func_0201cdd0(struct Board *board, unsigned char *base, int x, int y, unsigned char value);

int func_0201c7c8(struct Board *o)
{
    struct Code last;
    signed char dy;
    unsigned int row;
    unsigned short w;
    unsigned short pitch;
    struct Code code;
    unsigned short found;
    unsigned char lo;
    short ry;
    short y;
    short step;
    unsigned int i;
    unsigned int col;
    short rx;
    short yy;
    short x;
    unsigned char *base;
    signed char dx;
    unsigned char byte;
    unsigned char hi;
    unsigned int row2;

    base = o->base;
    x = o->cx + o->ox;
    y = (short)(o->oy + (o->line + o->gap) * o->scale);
    found = 0;
    if (o->count != 0) {
        for (i = 0; i < o->count; i++) {
            if (o->id == o->table[i].id) {
                found = o->table[i].width;
                break;
            }
        }
    }
    w = o->adv;
    func_0207c594(&last, 2);
    if (o->field != 0) {
        code = func_0200e410(o->field, o->id, o->sub);
        func_0201cabc(&last, &code);
        w = func_0201cf7c(o, o->id, o->sub);
    }
    if (func_0201cff0(o, o->id, o->sub)) {
        x += o->extra;
        w += o->extra;
    }
    step = found == 0 ? w : found;
    o->cx += step;
    dx = 0;
    dy = o->obj->vtbl->get(o->obj, o->id);
    if (last.lo != 0) {
        dx = -(signed char)last.lo;
        dy += (signed char)func_0201cfd0(o, o->id);
    }
    if (x + o->cell_w > o->max_x && func_0201ce60(o, x + dx, o->max_x) != 0) {
        return 2;
    }
    if (y + o->cell_h > o->max_y) {
        return 1;
    }
    pitch = o->width / 2;
    if (o->big) {
        x /= 2;
        step = found == 0 ? w : found;
        o->cx += step;
        y -= 8;
        for (row = 0; row < o->cell_h; row++) {
            ry = y + row * 2;
            if (ry < 0) {
                continue;
            }
            if (ry >= o->height - 1) {
                continue;
            }
            for (col = 0; col < o->cell_w / 2; col++) {
                rx = x + col * 2;
                if (rx < 0) {
                    continue;
                }
                if (rx >= pitch - 1) {
                    continue;
                }
                byte = o->data[(unsigned short)(col + row * o->cell_w / 2)];
                hi = byte & 0xf0;
                lo = byte & 0xf;
                if (hi != 0) {
                    func_0201ce3c(o, base + (rx + ry * pitch + 1), pitch, hi);
                }
                if (lo != 0) {
                    func_0201ce3c(o, base + (rx + ry * pitch), pitch, lo);
                }
            }
        }
    } else {
        for (row2 = 0; row2 < o->cell_h; row2++) {
            yy = dy + (y + row2);
            for (col = 0; col < o->cell_w / 2; col++) {
                rx = dx + (x + col * 2);
                byte = o->data[(unsigned short)(col + row2 * o->cell_w / 2)];
                if (byte != 0) {
                    hi = byte & 0xf0;
                    lo = byte & 0xf;
                    if (lo != 0) {
                        func_0201cdd0(o, base, rx, yy, lo);
                        if (o->used_x < rx + 1) {
                            o->used_x = rx + 1;
                        }
                        if (o->used_y < yy) {
                            o->used_y = yy;
                        }
                    }
                    if (hi != 0) {
                        func_0201cdd0(o, base, rx + 1, yy, hi >> 4);
                        if (o->used_x < rx) {
                            o->used_x = rx;
                        }
                        if (o->used_y < yy) {
                            o->used_y = yy;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
