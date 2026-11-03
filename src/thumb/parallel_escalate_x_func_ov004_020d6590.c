struct Stream6590 {
    char pad000[0x30];
    unsigned long long tick;
    short *left;
    short *right;
    int pos;
    unsigned int frames;
    char pad048[0xdc - 0x48];
    unsigned int wdc;
    unsigned int we0;
    char pad0e4[0xf4 - 0xe4];
    int step;
    char pad0f8[0x9d4 - 0xf8];
    int w9d4;
    char pad9d8[0xaa8 - 0x9d8];
    unsigned char aa8_low : 5;
    unsigned char waiting : 1;
    unsigned char aa8_high : 2;
    unsigned char aa9;
    unsigned char mode : 3;
    unsigned char kind : 2;
};

extern volatile unsigned long long data_020bbb58;
extern unsigned int func_02074500(void);
extern int func_ov004_020d3ce4(int a);
extern unsigned int func_ov004_020d3e7c(int a);
extern void func_ov004_020d3e94(int a, short *buf);
extern void func_02074860(void *buf, int size);
extern void func_ov004_020d5a04(struct Stream6590 *p);


void func_ov004_020d6590(struct Stream6590 *p)
{
    unsigned long long count;
    int i;

    if ((p->mode & 4) == 0) {
        return;
    }
    if (((func_02074500() & 0x40000000) || (func_02074500() & 0x80000000)) && func_ov004_020d3ce4(p->w9d4) != 0) {
        count = func_ov004_020d3e7c(p->w9d4);
        for (i = 0; i < count; i++) {
            if (p->waiting) {
                if (data_020bbb58 < p->tick) {
                    while (p->frames - (p->tick - data_020bbb58) == 0) {
                    }
                }
            }
            if (p->kind == 1) {
                func_ov004_020d3e94(p->w9d4, p->left + p->pos);
                func_02074860(p->left + p->pos, p->step * 2);
            } else {
                func_ov004_020d3e94(p->w9d4, p->left + p->pos);
                func_ov004_020d3e94(p->w9d4, p->right + p->pos);
                func_02074860(p->left + p->pos, p->step * 2);
                func_02074860(p->right + p->pos, p->step * 2);
            }
            p->pos += p->step;
            if (p->pos == p->frames * p->step) {
                p->pos = 0;
            }
            p->tick++;
        }
        if (!p->waiting && p->we0 >= p->wdc) {
            func_ov004_020d5a04(p);
            p->waiting = 1;
        }
    }
}
