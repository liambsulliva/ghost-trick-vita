struct Walker {
    char pad0[0x28];
    unsigned short id;
    char pad2a[0x11];
    unsigned char step;
    char pad3c[0x12];
    unsigned char count;
    char pad4f[5];
    int state;
};

extern void func_0201a8f0(struct Walker *w);

void func_0201a8a0(struct Walker *w)
{
    if (w->state != 1) {
        if (w->state == 2) {
            w->count++;
            return;
        }
        if (w->id == 0x111) {
            w->id = 0xf4;
        }
        if (w->step == 0 && w->id == 0xff) {
            return;
        }
        func_0201a8f0(w);
        w->step++;
    }
}
