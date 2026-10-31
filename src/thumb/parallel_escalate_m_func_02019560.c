struct Walker {
    char pad0[0x10];
    int link;
    char pad14[0x14];
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
    int layer;
    char pad64[4];
    int sheet;
    int active;
    char pad70[4];
    int boost;
};

extern char data_020b9df4[];
extern void func_0201c6c4(void *map, int link);
extern void func_0201c634(void *map, int sheet, int anim, int frame, int count);
extern void func_0201cf2c(void *map, int layer);
extern void func_02016fbc(void *map, int value);
extern void func_02016fc4(void *map, unsigned short value);
extern void func_020194bc(void *map, unsigned short value);
extern int func_0201cf7c(void *map, unsigned int id, unsigned short sub);
extern void func_0201cf10(void *map, unsigned int id, unsigned short sub, short x, short y, unsigned char mode, unsigned char kind);
extern int func_0201c710(void *map);
extern int func_0201cff0(void *map, unsigned int id, unsigned int sub);
extern int func_02017214(void *map);

void func_02019560(struct Walker *w)
{
    short x;
    int step;
    short y;

    if (w->active == 0) {
        return;
    }
    y = (int)w->off44 + (unsigned int)w->scale * ((unsigned int)w->rows + (int)w->off40);
    x = w->x + w->off42 + w->off3c + w->off3e;
    step = func_0201cf7c(data_020b9df4, w->id, w->sub);
    func_0201cf10(data_020b9df4, w->id, w->sub, x, y, w->mode, w->kind);
    w->state = func_0201c710(data_020b9df4);
    if (w->state == 2) {
        w->count++;
    }
    if (w->boost != 0) {
        w->x = w->x + (short)w->boost;
        w->x = w->x + (short)(func_02017214(data_020b9df4) * 2);
        w->boost = 0;
    } else {
        w->x = w->x + (short)step;
    }
    w->y36 = y + w->rows;
    if (w->kind != 0) {
        w->x = w->x + (short)step;
    }
    if (func_0201cff0(data_020b9df4, w->id, w->sub) != 0) {
        w->x = w->x + func_02017214(data_020b9df4);
    }
}
