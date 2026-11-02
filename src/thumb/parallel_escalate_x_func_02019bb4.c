struct Walker {
    char pad0[0x28];
    unsigned short id;
    unsigned short mode;
    char pad2c[4];
    short off30;
    char pad32[2];
    short x;
    short y36;
    char pad38[2];
    unsigned char scale;
    char pad3b;
    short off3c;
    short off3e;
    short off40;
    short off42;
    short off44;
    unsigned short anim;
    unsigned short frame;
    char pad4a[4];
    unsigned char count;
    char pad4f[2];
    unsigned char rows;
    char pad52[2];
    int state;
    char pad58[2];
    unsigned char kind;
    char pad5b[3];
    unsigned short sub;
    char pad60[8];
    void *table;
    char pad6c[4];
    unsigned char index;
};

extern char data_020b9df4[];
extern int func_02019cd8(void *table);
extern int func_02019a58(void *table, unsigned int index);
extern int func_02019cdc(void *table, unsigned int index, int value);
extern void func_0201c634(void *map, int sheet, int anim, int frame, int count);
extern void func_02016fc4(void *map, unsigned short value);
extern void func_0201cf10(void *map, unsigned int id, unsigned short sub, short x, short y, unsigned char mode, unsigned char kind);
extern int func_0201c710(void *map);
extern int func_0201cf7c(void *map, unsigned int id, unsigned short sub);
extern int func_0201cff0(void *map, unsigned int id, unsigned int sub);
extern int func_02017214(void *map);

void func_02019bb4(struct Walker *w)
{
    unsigned int index;
    int sheet;
    int offset;
    int base;
    short y;
    short x;
    int step;

    index = w->index;
    if (index < 3) {
        sheet = func_02019cd8(w->table);
        offset = index * 0x6000;
        index = func_02019a58(w->table, w->index);
        base = func_02019cdc(w->table, w->index, index);
        y = base + w->scale * (w->rows + w->off40);
        x = w->x + w->off42 + w->off3c;
        func_0201c634(data_020b9df4, sheet + offset, w->anim, w->frame, 2);
        func_02016fc4(data_020b9df4, w->anim - w->off30);
        func_0201cf10(data_020b9df4, w->id, w->sub, x, y, w->mode, w->kind);
        w->state = func_0201c710(data_020b9df4);
        step = func_0201cf7c(data_020b9df4, w->id, w->sub);
        if (w->state == 2) {
            w->count++;
        }
        w->x = w->x + (short)step;
        w->y36 = y + w->rows;
        if (func_0201cff0(data_020b9df4, w->id, w->sub) != 0) {
            w->x = w->x + func_02017214(data_020b9df4);
        }
    }
}
