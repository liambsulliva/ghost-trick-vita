struct Walker {
    char pad0[0x28];
    unsigned short id;
    unsigned short mode;
    char pad2c[8];
    short x;
    char pad36[0x18];
    unsigned char count;
    char pad4f[5];
    int state;
    char pad58[2];
    unsigned char kind;
    char pad5b[3];
    unsigned short sub;
};

extern char data_020b9df4[];
extern int func_0201cf7c(void *map, unsigned int id, unsigned short sub);
extern void func_0201cf10(void *map, unsigned int id, unsigned short sub, short x, short y, unsigned char mode, unsigned char kind);
extern int func_0201c710(void *map);
extern int func_0201cff0(void *map, unsigned int id, unsigned int sub);
extern int func_02017214(void *map);

void func_0201a8f0(struct Walker *w)
{
    unsigned short x;
    int step;


    x = w->x;
    step = func_0201cf7c(data_020b9df4, w->id, w->sub);
    func_0201cf10(data_020b9df4, w->id, w->sub, x, 0, w->mode, w->kind);
    w->state = func_0201c710(data_020b9df4);
    if (w->state == 2) {
        w->count++;
    }

    w->x = w->x + (short)step;
    if (func_0201cff0(data_020b9df4, w->id, w->sub) != 0) {
        w->x = w->x + func_02017214(data_020b9df4);
    }
}
