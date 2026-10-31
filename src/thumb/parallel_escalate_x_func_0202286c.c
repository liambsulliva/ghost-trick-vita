struct Pad {
    int disabled;
    int word4;
    unsigned short cont;
    unsigned short trig;
    unsigned short rel;
    unsigned short half_e;
    unsigned short raw;
    unsigned short prev;
};

struct State {
    unsigned int cur;
    unsigned int trig;
    unsigned int rel;
    unsigned int word_c;
    unsigned int flags;
    unsigned int prev;
    unsigned int fold;
};

extern struct Pad data_020c9d38;
extern struct State data_020c9d50;
extern unsigned char data_020c9d90[];
extern int func_02022918(void);
extern unsigned int func_0202292c(void);
extern unsigned int func_020229e8(unsigned int);
extern void func_0202296c(void);

void func_0202286c(void)
{
    int fold;
    unsigned int keys;
    unsigned int cont;
    int i;

    keys = 0;
    fold = func_02022918();
    if (fold != 0) {
        cont = 0;
    } else {
        if (data_020c9d38.disabled == 0) {
            keys = func_0202292c();
        }
        cont = keys;
        if (data_020c9d38.disabled == 0) {
            cont = keys | func_020229e8(data_020c9d50.flags);
        }
    }
    data_020c9d38.prev = data_020c9d38.cont;
    data_020c9d38.cont = cont;
    data_020c9d38.rel = ~data_020c9d38.cont & (data_020c9d38.cont ^ data_020c9d38.prev);
    data_020c9d38.trig = ~data_020c9d38.prev & cont;
    data_020c9d38.raw = keys;
    data_020c9d50.fold = fold;
    if (*(unsigned char *)((int)data_020c9d90 + 0x1c) == 0) {
        data_020c9d50.prev = data_020c9d50.cur;
        data_020c9d50.cur = data_020c9d50.flags;
        for (i = 0; i < 32; i++) {
            if ((*(unsigned short *)((char *)&data_020c9d50 + i * 2 + 0x1c) & keys) != 0) {
                data_020c9d50.cur |= 1 << i;
            }
        }
        data_020c9d50.rel = (data_020c9d50.prev ^ data_020c9d50.cur) & ~data_020c9d50.cur;
        data_020c9d50.trig = ~data_020c9d50.prev & data_020c9d50.cur;
        data_020c9d50.flags = 0;
    }
    func_0202296c();
}
