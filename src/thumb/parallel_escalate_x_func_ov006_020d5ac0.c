struct Keys {
    unsigned int held;
    unsigned int pressed;
};

struct Item {
    short v[7];
};

struct Page {
    char pad0[0x20];
    int mode;
    unsigned char state;
    unsigned char page;
    unsigned char step;
    unsigned char sound;
    signed char slide;
    unsigned char hold;
    char pad2a;
    signed char timer;
    char pad2c[2];
    short target;
    short scroll;
    signed char sel[2];
    signed char next[2];
    signed char count[2];
    char pad38[0x3a4];
    short drag_x;
    short drag_y;
    char pad3e0[2];
    short touch_x;
    short touch_y;
    char pad3e6[6];
    unsigned char inside;
    char pad3ed[0x23];
    void *button_ok;
    void *button_back;
    void *frame_top;
    void *frame_bottom;
    void *arrow_left;
    void *arrow_right;
    void *hint;
    char pad42c[0x230];
    struct Item *items;
};

extern struct Keys data_020c9d50;
extern char data_ov006_020d8df4[];
extern void func_ov006_020d7be8(struct Page *p, void *input);
extern int func_ov006_020d5d54(void *button);
extern int func_ov006_020d5d64(void *button);
extern int func_ov006_020d5d74(void *button);
extern void func_ov006_020d7fc0(void *button, int value);
extern void *func_0200847c(void);
extern void func_0202a0c0(void *sound, int id);
extern void func_020521d4(char *fade, int screen, int a, int b);
extern void func_ov006_020d4268(void *input);
extern int func_ov006_020d5d80(struct Item *item);
extern void func_ov006_020d6c20(struct Page *p);
extern void func_0202f64c(void *slot);
extern int func_ov006_020d7320(struct Page *p, int a, int index, int count, void *left, void *right);
extern void func_ov006_020d7d74(struct Page *p, void *input);
extern void *func_02010b7c(void);
extern int func_02015188(void *game);
extern int func_0202cb64(void *game);
extern int func_02010b60(void *game);
extern int func_02010b64(void *game);

void func_ov006_020d5ac0(struct Page *p, void *input)
{
    unsigned char side;
    int flag;
    int hit;
    int moved;
    short x;
    short y;

    side = p->page & 1;
    if (p->timer != 0) {
        p->timer = p->timer - 1;
        if (p->timer <= 0) {
            func_ov006_020d7be8(p, input);
        }
    }
    if (p->button_back != 0) {
        if (func_ov006_020d5d54(p->button_back) != 0) {
            func_0202a0c0(func_0200847c(), 2);
            p->sound = 1;
            p->state = 9;
            func_020521d4(data_ov006_020d8df4, 0, 6, 0x100);
            func_020521d4(data_ov006_020d8df4, 1, 6, 0x100);
            return;
        }
        if (func_ov006_020d5d64(p->button_back) != 0) {
            return;
        }
        if ((data_020c9d50.pressed & 4) != 0 || func_ov006_020d5d74(p->button_back) != 0) {
            func_ov006_020d7fc0(p->button_back, 0);
            return;
        }
    }
    if (func_ov006_020d5d54(p->button_ok) != 0) {
        if (p->mode == 0) {
            func_ov006_020d4268(input);
            p->state = 8;
        } else {
            p->state = 9;
            func_020521d4(data_ov006_020d8df4, 0, 6, 0x100);
            func_020521d4(data_ov006_020d8df4, 1, 6, 0x100);
        }
        p->sound = 0;
        func_0202a0c0(func_0200847c(), 9);
        return;
    }
    if (func_ov006_020d5d64(p->button_ok) != 0) {
        return;
    }
    if ((data_020c9d50.pressed & 0x2000) != 0 || func_ov006_020d5d74(p->button_ok) != 0) {
        func_ov006_020d7fc0(p->button_ok, 0);
        return;
    }
    hit = 0;
    if (p->items != 0) {
        hit = func_ov006_020d5d80(&p->items[1 - p->page]);
    }
    flag = 0;
    if ((data_020c9d50.pressed & 0x30) != 0 || hit != 0) {
        flag = 1;
        func_0202a0c0(func_0200847c(), 10);
    }
    if (flag != 0) {
        p->page = (side + 1) & 1;
        func_ov006_020d4268(input);
        func_ov006_020d6c20(p);
        p->drag_x = 0;
        p->drag_y = 0;
        p->step = 7;
        func_0202f64c(&p->hint);
        return;
    }
    moved = func_ov006_020d7320(p, 0, p->sel[side], p->count[side], p->frame_top, p->frame_bottom);
    if (moved != 0) {
        p->scroll = 0;
        p->target = -(moved << 7);
        p->step = 1;
        p->slide = 0;
        p->hold = 0;
        func_0202a0c0(func_0200847c(), 1);
        func_ov006_020d4268(input);
        func_0202f64c(&p->hint);
        return;
    }
    if ((data_020c9d50.pressed & 0x1000) != 0) {
        func_ov006_020d7d74(p, input);
        return;
    }
    if (func_02015188(func_02010b7c()) != 0 && func_0202cb64(func_02010b7c()) == 0) {
        x = func_02010b60(func_02010b7c());
        y = func_02010b64(func_02010b7c());
        p->inside = 0;
        if (y > 0x26 && y < 0x9a) {
            p->step = 9;
            p->touch_x = x;
            p->touch_y = y;
            if (x > 0x44 && x < 0xbc && y > 0x2e && y < 0x92) {
                p->inside = 1;
            }
        }
    }
}
