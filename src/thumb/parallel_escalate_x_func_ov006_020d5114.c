struct Bytes1 {
    signed char v[1];
};

struct Bytes2 {
    signed char v[2];
};

struct Bytes4 {
    signed char v[4];
};

struct Order {
    short v[33];
};

struct Entry {
    int id;
    short *value;
};

struct Fade {
    char pad0[0x26];
    short level;
};

struct Page {
    char pad0[0x20];
    int mode;
    unsigned char state;
    char pad25[0x11];
    signed char count[2];
    signed char rows[2][50];
    int values[2][50];
    int marks[2][50];
    unsigned char open[23];
    char pad3d3[0x291];
    int busy;
    char list[4];
};

extern struct Fade data_020c9ec8;
extern char data_ov006_020d8df4[];
extern const struct Bytes1 data_ov006_020d8988;
extern const struct Bytes2 data_ov006_020d898c;
extern const struct Bytes2 data_ov006_020d8990;
extern const struct Bytes4 data_ov006_020d8994;
extern const struct Order data_ov006_020d8c94;
extern struct Entry data_ov006_020db338[];
extern struct Entry data_ov006_020db280[];
extern char data_020cd878[];
extern char data_020cd894[];
extern void *func_02008444(void);
extern int func_02010fbc(void *game, int a);
extern void func_02010ef4(void *game, int a, int b);
extern int func_02010ff4(void *game, int a);
extern void func_020521d4(char *fade, int screen, int a, int b);
extern void func_ov006_020d542c(char *list);
extern int func_ov006_020d74a4(struct Page *p, unsigned char side, signed char index);
extern unsigned char func_ov006_020d7504(struct Page *p, signed char index);
extern void *func_02022dfc(struct Page *p);
extern void func_02022f2c(void *manager, char *name, int value);

void func_ov006_020d5114(struct Page *p)
{
    struct Order order;
    struct Bytes4 first0;
    signed char *first[2];
    struct Bytes2 limits;
    struct Bytes2 counts;
    struct Bytes1 first1;
    signed char side;
    signed char k;
    signed char j;
    int id;

    if (func_02010fbc(func_02008444(), 3) != 0 && p->mode == 0) {
        func_02010ef4(func_02008444(), 0xff, -1);
        return;
    }
    if (func_02010ff4(func_02008444(), 3) == 0 && p->mode == 0) {
        return;
    }
    if (data_020c9ec8.level != -16) {
        func_020521d4(data_ov006_020d8df4, 0, 6, 0x100);
        func_020521d4(data_ov006_020d8df4, 1, 6, 0x100);
    }
    p->busy = 0;
    func_ov006_020d542c(p->list);
    p->state = 2;
    limits = data_ov006_020d8990;
    counts = data_ov006_020d898c;
    first0 = data_ov006_020d8994;
    first1 = data_ov006_020d8988;
    first[0] = first0.v;
    first[1] = first1.v;
    order = data_ov006_020d8c94;
    for (side = 0; side < 2; side++) {
        p->count[side] = 0;
        for (k = 0; k < limits.v[side]; k++) {
            if (side == 0) {
                id = order.v[k];
                if (p->mode != 0) {
                    p->marks[side][id - 1] = 0;
                    p->values[side][id - 1] = 10;
                    if (data_ov006_020db338[id - 1].value != 0) {
                        p->values[side][id - 1] = *data_ov006_020db338[id - 1].value;
                    }
                } else {
                    p->values[side][id - 1] = func_ov006_020d74a4(p, side, id - 1);
                    if (p->values[side][id - 1] < 0) {
                        continue;
                    }
                }
                if (id == 2 || id == 4 || id == 5 || id == 7 || id == 9 || id == 16 || id == 19 || id == 21 || id == 35 || id == 36 || id == 37) {
                    continue;
                }
                p->rows[side][p->count[side]] = id - 1;
                p->count[side]++;
            }
            if (side == 1) {
                if (p->mode != 0) {
                    p->marks[side][k] = 0;
                    p->values[side][k] = 10;
                    if (data_ov006_020db280[k].value != 0) {
                        p->values[side][k] = *data_ov006_020db280[k].value;
                    }
                } else {
                    p->values[side][k] = func_ov006_020d74a4(p, side, k);
                }
                if (p->values[side][k] < 0) {
                    continue;
                }
                if (k + 1 == 5 || k + 1 == 6 || k + 1 == 7 || k + 1 == 8 || k + 1 == 9 || k + 1 == 10 || k + 1 == 17 || k + 1 == 19 || k + 1 == 23) {
                    continue;
                }
                p->rows[side][p->count[side]] = k;
                p->count[side]++;
            }
        }
        if (p->count[side] == 0) {
            for (j = 0; j < counts.v[side]; j++) {
                p->rows[side][j] = first[side][j] - 1;
                p->values[side][first[side][j] - 1] = 10;
            }
            p->count[side] = counts.v[side];
        }
    }
    for (k = 0; k < 23; k++) {
        p->open[k] = func_ov006_020d7504(p, k);
    }
    if (p->mode == 1) {
        func_02022f2c(func_02022dfc(p), data_020cd878, 0);
    } else {
        func_02022f2c(func_02022dfc(p), data_020cd894, 0);
    }
}
