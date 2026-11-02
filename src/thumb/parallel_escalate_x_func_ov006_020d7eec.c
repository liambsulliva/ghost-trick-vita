typedef struct Anim7eec {
    char pad0[0x28];
    int sprite[0x25];
    unsigned char count;
    unsigned char done;
    signed char frame;
    unsigned char alt;
    char padc0[8];
    int target;
    int current;
    int normal;
    int padd4;
    int other;
} Anim7eec;

extern void func_0200b598(int *sprite, int value, int mode);
extern int func_0200bbe8(int *sprite);
extern int func_0200ad10(int a, int frame);
extern int func_02009c1c(int a);
extern int func_0200ba2c(int *sprite);
extern int func_0200ba44(int *sprite);
extern void func_0202f1bc(int a, int b, int c, int d, int e);
extern void func_0200b7c0(int *sprite);

void func_ov006_020d7eec(Anim7eec *p)
{
    int base;
    int obj;
    int w;
    int h;

    p->done = 0;
    if (p->current != p->target) {
        p->current = p->target;
        func_0200b598(p->sprite, p->target, 15);
    }
    if (p->count != 0) {
        p->count++;
        if (p->count > 12) {
            p->count = 0;
            p->done = 1;
            if (p->alt) {
                p->target = p->other;
                p->current = p->target;
                func_0200b598(p->sprite, p->target, 15);
            } else {
                p->target = p->normal;
                p->current = p->target;
                func_0200b598(p->sprite, p->target, 15);
            }
        }
    } else if (p->frame >= 0) {
        base = func_0200bbe8(p->sprite);
        obj = func_02009c1c(func_0200ad10(base, p->frame));
        w = func_0200ba2c(p->sprite);
        h = func_0200ba44(p->sprite);
        func_0202f1bc(obj, base, w << 5, h, 0x12);
    }
    func_0200b7c0(p->sprite);
}
