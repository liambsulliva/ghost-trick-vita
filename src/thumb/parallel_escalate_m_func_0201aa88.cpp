class Base {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual int step();
};
struct Slot {
    int cell;
    unsigned short width;
    unsigned char off;
    unsigned char on;
};
struct Grid : public Base {
    unsigned char pad04[0x30 - 0x04];
    short x0;
    short y0;
    unsigned char pad34[0x38 - 0x34];
    unsigned short count;
    unsigned char pad3a[0x40 - 0x3a];
    short gap;
    short x1;
    short y1;
    unsigned short span;
    unsigned char pad48[0x51 - 0x48];
    unsigned char row;
    unsigned char pad52[0x68 - 0x52];
    char *buf;
    unsigned char pad6c[0xe0 - 0x6c];
    Slot slots[8];
};
extern "C" {
unsigned char func_0201ab84(Grid *grid, int cell, unsigned int idx);
void func_0201aba0(Grid *grid, char *buf, unsigned int pos, unsigned char value);

void func_0201aa88(Grid *g)
{
    unsigned char i;
    unsigned char j;
    unsigned int k;
    int bx;
    int by;
    int cell;
    unsigned int idx;
    unsigned int pos;
    unsigned char r;

    for (i = 0; i < g->count; i++) {
        if (g->slots[i].on) {
            g->slots[i].off = (g->span - (unsigned short)(g->slots[i].width + 1)) / 2;
        }
        bx = g->x0 + g->x1 + g->slots[i].off;
        by = g->y0 + g->y1 + i * (g->row + g->gap);
        g->step();
        cell = g->slots[i].cell;
        for (j = 0; j < 0x14; j++) {
            for (k = 0; k < 0x100; k++) {
                idx = k + j * 0x100;
                pos = k + (bx + g->step() * (by + j));
                if (idx <= 0x1400 && pos <= 0x4000) {
                    r = func_0201ab84(g, cell, idx);
                    if (r != 0) {
                        func_0201aba0(g, g->buf + 0x24, pos, r);
                    }
                }
            }
        }
    }
}
}
