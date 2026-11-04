struct Entry {
    int count;
    short *value;
};

struct Tables {
    struct Entry *e[2];
};

struct Keys {
    unsigned int held;
    unsigned int pressed;
};

struct Display {
    char pad0[0x54];
    unsigned int flags;
};

struct Page {
    char pad0[0x25];
    unsigned char page;
    unsigned char step;
    char pad27[4];
    signed char timer;
    char pad2c[6];
    signed char sel[2];
    signed char next[2];
    signed char count[2];
    signed char rows[2][50];
    int values[2][50];
    int marks[2][50];
    unsigned char open[0x1c];
    short shift;
    short offset;
    char pad3dc[4];
    short blink;
    char pad3e2[7];
    unsigned char dir;
    unsigned char redraw;
    signed char moved;
    char pad3ec[0x2c];
    void *frame_top;
    void *frame_bottom;
    void *arrow_left;
    void *arrow_right;
};

extern struct Display data_020c9e48;
extern struct Keys data_020c9d50;
extern const struct Tables data_ov006_020d8ac0;
extern const struct Tables data_ov006_020d8a98;
extern void func_ov006_020d789c(struct Page *p, int x, int y);
extern signed char func_ov006_020d7320(struct Page *p, int a, signed char index, signed char count, void *left, void *right);
extern void func_ov006_020d7cf8(struct Page *p, int pos, int limit);
extern unsigned int func_ov006_020d5f78(void *button);
extern void func_ov006_020d7c8c(struct Page *p, void *top, void *bottom, int sel, signed char count);
extern void func_ov006_020d420c(void *object);
extern void func_ov006_020d7108(struct Page *p, void *input, int mode);
extern void func_ov006_020d4098(void *object, unsigned char bit, signed char cell, unsigned char flag, unsigned int value);

void func_ov006_020d66bc(struct Page *p, void *input)
{
    unsigned char side;
    struct Tables tables;
    struct Tables totals;
    int item;
    signed char moved;
    signed char pos;
    signed char limit;
    signed char now;
    int total;
    int mark;
    int touch_left;
    int touch_right;
    int key_x;
    int key_y;
    int touch_top;
    int touch_bottom;
    int key_right;
    int key_left;
    signed char *base;
    signed char *sel;
    signed char cur;
    signed char cell;
    unsigned char flag;

    side = p->page & 1;
    p->blink++;
    if (p->blink == 0) {
        p->blink = 0x20;
    } else if (p->blink % 6 < 3 || p->blink > 0x20) {
        data_020c9e48.flags |= 0x200;
    } else {
        data_020c9e48.flags &= ~0x200;
    }
    if (p->blink >= 0x20) {
        if (p->blink == 0x20) {
            p->timer = 0;
        }
        data_020c9e48.flags &= ~0x200;
        if (p->redraw == 1) {
            if (p->moved == 1) {
                p->shift += 16;
            } else {
                p->shift -= 16;
            }
            func_ov006_020d789c(p, p->shift, p->offset);
        }
        if ((p->shift > 0x100 && p->moved == 1) || (p->shift < -0x100 && p->moved == -1)) {
            p->redraw = 0;
            func_ov006_020d789c(p, 0, 0);
        }
        if (p->redraw != 0) {
            return;
        }
        moved = 0;
        pos = -1;
        limit = -1;
        base = p->sel;
        sel = base;
        sel += side;
        cur = base[side];
        item = p->rows[side][cur];
        if (p->dir == 1) {
            limit = p->count[side];
            pos = cur;
            moved = func_ov006_020d7320(p, 1, cur, limit, p->frame_top, p->frame_bottom);
            if (moved != 0) {
                p->next[side] = p->sel[side] = pos + moved;
            }
        }
        if (p->dir == 2) {
            tables = data_ov006_020d8ac0;
            limit = tables.e[side][item].count;
            pos = p->marks[side][item];
            moved = func_ov006_020d7320(p, 0, pos, limit, p->arrow_left, p->arrow_right);
            if (moved != 0) {
                now = pos + moved;
                p->values[side][item] = tables.e[side][item].value[now];
                p->marks[side][item] = now;
                func_ov006_020d7cf8(p, pos, limit);
            }
        }
        totals = data_ov006_020d8a98;
        total = totals.e[side][item].count;
        mark = p->marks[side][item];
        touch_left = 0;
        touch_right = 0;
        key_x = 0;
        key_y = 0;
        touch_top = 0;
        touch_bottom = 0;
        key_right = 0;
        key_left = 0;
        if (p->arrow_left != 0 && func_ov006_020d5f78(p->arrow_left) != 0) {
            touch_left = 1;
        }
        if (p->arrow_right != 0 && func_ov006_020d5f78(p->arrow_right) != 0) {
            touch_right = 1;
        }
        if (data_020c9d50.held & 0x400) {
            key_x = 1;
        }
        if (data_020c9d50.held & 0x800) {
            key_y = 1;
        }
        if (p->frame_top != 0 && func_ov006_020d5f78(p->frame_top) != 0) {
            touch_top = 1;
        }
        if (p->frame_bottom != 0 && func_ov006_020d5f78(p->frame_bottom) != 0) {
            touch_bottom = 1;
        }
        if (data_020c9d50.held & 0x10) {
            key_right = 1;
        }
        if (data_020c9d50.held & 0x20) {
            key_left = 1;
        }
        if (moved != 0) {
            p->blink = -1;
            p->redraw = 1;
            p->moved = moved;
            func_ov006_020d7cf8(p, mark, total);
        } else if (key_x && pos > 0 && p->dir == 2) {
            func_ov006_020d7cf8(p, mark, total);
        } else if (key_y && pos < limit - 1 && p->dir == 2 && !key_x) {
            func_ov006_020d7cf8(p, mark, total);
        } else if (touch_left && pos > 0 && p->dir == 2 && !key_y) {
            func_ov006_020d7cf8(p, mark, total);
        } else if (touch_right && pos < limit - 1 && p->dir == 2 && !key_x && !touch_left) {
            func_ov006_020d7cf8(p, mark, total);
        } else if (key_right && pos > 0 && p->dir == 1) {
            func_ov006_020d7cf8(p, mark, total);
            func_ov006_020d7c8c(p, p->frame_top, p->frame_bottom, *sel, p->count[side]);
        } else if (key_left && pos < limit - 1 && p->dir == 1 && !key_right) {
            func_ov006_020d7cf8(p, mark, total);
            func_ov006_020d7c8c(p, p->frame_top, p->frame_bottom, *sel, p->count[side]);
        } else if (touch_top && pos > 0 && p->dir == 1 && !key_left) {
            func_ov006_020d7cf8(p, mark, total);
            func_ov006_020d7c8c(p, p->frame_top, p->frame_bottom, *sel, p->count[side]);
        } else if (touch_bottom && pos < limit - 1 && p->dir == 1 && !key_right && !touch_top) {
            func_ov006_020d7cf8(p, mark, total);
            func_ov006_020d7c8c(p, p->frame_top, p->frame_bottom, *sel, p->count[side]);
        } else {
            func_ov006_020d7cf8(p, mark, total);
            func_ov006_020d7c8c(p, p->frame_top, p->frame_bottom, p->sel[side], p->count[side]);
            func_ov006_020d420c(input);
            func_ov006_020d7108(p, input, 0);
            p->step = 0;
            flag = 0;
            cell = p->rows[side][p->sel[side]];
            if (side != 0 && p->open[cell] == 0) {
                flag = 1;
            }
            func_ov006_020d4098(input, side, cell, flag, p->values[side][cell]);
            func_ov006_020d7108(p, input, 1);
        }
    }
}
