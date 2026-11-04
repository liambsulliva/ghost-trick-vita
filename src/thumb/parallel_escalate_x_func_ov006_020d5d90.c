struct Keys {
    unsigned int held;
};

struct Page {
    char pad0[0x26];
    unsigned char state;
    char pad27;
    signed char step;
    unsigned char hold;
    char pad2a[4];
    short target;
    short scroll;
    signed char sel[2];
    signed char next[2];
    signed char count[2];
    char pad38[0x3ae];
    short slot;
    char pad3e8[0x30];
    void *arrow_left;
    void *arrow_right;
};

extern struct Keys data_020c9d50;
extern void func_ov006_020d79b4(struct Page *p, void *input, int sel, int a, int b, int scroll);
extern int func_ov006_020d7b34(struct Page *p, void *input);
extern int func_ov006_020d7320(struct Page *p, int a, int index, int count, void *left, void *right);
extern int func_ov006_020d5f78(void *arrow);
extern int func_ov006_020d42b8(void *input);
extern signed char func_ov006_020d7b88(struct Page *p, void *input);
extern void func_ov006_020d7704(struct Page *p, void *input, int side, int a);
extern void func_ov006_020d78c4(struct Page *p, int side, int slot);

void func_ov006_020d5d90(struct Page *p, void *input)
{
    unsigned char side;
    unsigned char left_key;
    int index;
    int moved;
    unsigned char right_key;
    unsigned char left_touch;
    unsigned char right_touch;
    int next;
    signed char *sel;

    side = *((unsigned char *)p + 0x25) & 1;
    if (p->step > 4) {
        sel = &p->sel[side];
        left_key = 0;
        func_ov006_020d79b4(p, input, *sel, 1, 0, p->scroll);
        index = func_ov006_020d7b34(p, input);
        moved = func_ov006_020d7320(p, 0, index, p->count[side], p->arrow_left, p->arrow_right);
        right_key = 0;
        left_touch = 0;
        right_touch = 0;
        if (data_020c9d50.held & 0x400) {
            left_key = 1;
        }
        if (data_020c9d50.held & 0x800) {
            right_key = 1;
        }
        if (p->arrow_left != 0 && func_ov006_020d5f78(p->arrow_left) != 0) {
            left_touch = 1;
        }
        if (p->arrow_right != 0 && func_ov006_020d5f78(p->arrow_right) != 0) {
            right_touch = 1;
        }
        if (moved != 0) {
            p->scroll = p->target;
            p->target -= (short)(moved << 7);
            p->step = 0;
            p->hold = 0;
        } else if (func_ov006_020d42b8(input) != 0
            && !(left_key && index > 0)
            && !(right_key && index < p->count[side] - 1 && !left_key)
            && !(left_touch && index > 0 && !right_key)
            && !(right_touch && index < p->count[side] - 1 && !left_key && !left_touch)) {
            p->hold++;
            if (p->hold > 8) {
                next = func_ov006_020d7b88(p, input);
                p->next[side] = next;
                *sel = p->next[side];
                func_ov006_020d7704(p, input, side, 1);
                func_ov006_020d78c4(p, side, p->slot);
                p->state = 2;
            }
        }
    } else {
        func_ov006_020d79b4(p, input, p->sel[side], 1, 0, p->scroll);
    }
    if (p->scroll < p->target - 0x10) {
        p->scroll += 0x20;
    } else if (p->scroll > p->target + 0x10) {
        p->scroll -= 0x20;
    }
    if (p->scroll >= p->target - 0x10 && p->scroll <= p->target + 0x10) {
        p->scroll = p->target;
        p->step = 5;
    }
}
