typedef struct Board {
    char pad0[0x28];
    unsigned char b28;
    unsigned char b29;
    signed char state;
    signed char prev;
    signed char pending;
    signed char sel;
    unsigned char b2e;
    char pad2f;
    char sub30[8];
    unsigned int w38;
    unsigned int on;
    unsigned int off;
    unsigned int skip;
    void *parts[12];
} Board;

typedef struct Ids {
    unsigned char v[2];
} Ids;

extern const Ids data_0209be58;
extern char data_0209beb4[];
extern char data_0209bed0[];
extern char data_0209bec0[];
extern char data_0209bec8[];
extern void *func_02008444(void);
extern int func_0204faf4(void *part);
extern void func_0204fafc(void *part, int state);
extern void func_0204f290(void *part, int a, int b);
extern void func_0200da04(char *a, char *b, char *c, char *d);
extern void func_020113f8(void *game);
extern void func_02032644(void *game, int value);
extern void func_0204fdc4(Board *b, unsigned char mode, int flag);
extern unsigned char func_020100bc(void *game);
extern unsigned char func_0201008c(void *game);
extern void func_0204fbfc(Board *b, int idx, int mode, unsigned char x, unsigned char y);
extern unsigned char func_02010fa8(Board *b, signed char idx);
extern unsigned char func_0200acc8(void *sub);
extern void func_0204fb04(void *part, unsigned char *value);

void func_0204f850(Board *b)
{
    Ids ids;
    unsigned char now[2];
    unsigned char cur[2];
    unsigned int i;
    int j;
    int found;
    unsigned int id;
    unsigned char state;

    switch (b->state) {
    case 0:
        break;
    case 1:
    case 2:
        if (b->sel >= 0) {
            if (func_0204faf4(b->parts[b->sel]) != 4) {
                switch (b->sel) {
                case 5:
                    func_0200da04(data_0209beb4, data_0209bed0, data_0209bec0, data_0209bec8);
                    func_020113f8(func_02008444());
                    func_02032644(func_02008444(), 4);
                case 4:
                    b->state = 0;
                    break;
                default:
                    func_020113f8(func_02008444());
                    break;
                }
                b->sel = -1;
            }
        } else {
            for (i = 0; i < 12; i++) {
                if (func_0204faf4(b->parts[i]) != 0 && (b->state != 2 || i != 1) && (b->skip & (1 << i)) == 0) {
                    if (i == 4 && func_0204faf4(b->parts[i]) == 4) {
                        func_0204fafc(b->parts[i], 3);
                    } else {
                        func_0204f290(b->parts[i], 3, -1);
                    }
                }
            }
            b->prev = b->state;
            b->state = 0;
            if (b->pending > 0) {
                b->state = b->pending;
                b->prev = b->state;
                b->pending = 0;
            }
        }
        break;
    default:
        found = 0;
        for (i = 0; i < 12; i++) {
            if (func_0204faf4(b->parts[i]) == 3 && (b->skip & (1 << i)) == 0) {
                found = 1;
            }
        }
        if (found == 0) {
            func_0204fdc4(b, b->state - 2, 1);
            b->state = 0;
            b->skip = 0;
        }
        break;
    }
    b->on = 0;
    for (i = 0; i < 12; i++) {
        if (func_0204faf4(b->parts[i]) == 1) {
            b->on |= 1 << i;
        }
    }
    b->off = 0;
    for (i = 0; i < 12; i++) {
        if (func_0204faf4(b->parts[i]) == 0) {
            b->off |= 1 << i;
        }
    }
    ids = data_0209be58;
    now[0] = func_020100bc(func_02008444());
    now[1] = func_0201008c(func_02008444());
    cur[0] = b->b28;
    cur[1] = b->b29;
    for (j = 0; j < 2; j++) {
        if (now[j] != 0) {
            id = ids.v[j];
            state = func_0204faf4(b->parts[id]);
            if (state != 4) {
                func_0204fbfc(b, id, 0, 1 - j, j);
            }
        } else if (cur[j] != 0 && (b->prev == 3 || b->prev == 4) && (b->prev != 4 || ids.v[j] != 4)) {
            func_0204fbfc(b, ids.v[j], 1, 1 - j, j);
        }
    }
    for (i = 0; i < 12; i++) {
        if (func_02010fa8(b, i) != 0) {
            state = func_0204faf4(b->parts[i]);
            if (state == 2 || state == 3 || state == 4) {
                func_020113f8(func_02008444());
                break;
            }
        }
    }
    b->b2e = func_0200acc8(b->sub30);
    for (i = 0; i < 12; i++) {
        if (b->parts[i] != 0) {
            func_0204fb04(b->parts[i], &b->b2e);
        }
    }
}
