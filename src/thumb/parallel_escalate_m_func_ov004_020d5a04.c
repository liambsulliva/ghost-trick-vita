struct Panel {
    unsigned char pad000[0x38];
    unsigned int w38;
    unsigned int w3c;
    unsigned char pad040[4];
    unsigned int w44;
    unsigned char pad048[0xf4 - 0x48];
    unsigned int wf4;
    unsigned char pad0f8[0x9d4 - 0xf8];
    int w9d4;
    unsigned char pad9d8[0xaaa - 0x9d8];
    unsigned char mode:3;
    unsigned char kind:2;
};

extern unsigned int func_ov004_020d3ce4(int a);
extern void func_ov004_020d6be0(void);
extern void func_02074860(unsigned int start, unsigned int size);
extern void func_02076640(int a, int b);
extern void func_020766c8(int a, int b, unsigned int c, int d, int e, unsigned int f, int g, int h, unsigned int i, int j);
extern void func_020765d0(int a, unsigned int b, unsigned int c, void *d, int e);
extern void func_02076504(int a, int b, int c, int d);
extern void func_02076b60(int a);

void func_ov004_020d5a04(struct Panel *param_1)
{
    unsigned int step;
    unsigned int size;
    unsigned int scale;

    if ((param_1->mode & 4) != 0) {
        step = 0xffb0ff / func_ov004_020d3ce4(param_1->w9d4);
        size = param_1->wf4;
        scale = (size >> 6) * (step * 2);
        if (param_1->kind == 1) {
            func_02074860(param_1->w38, param_1->w44 * size * 2);
            func_02076640(1, 0);
            func_020766c8(0, 1, param_1->w38, 1, 0, (param_1->w44 * param_1->wf4) >> 1, 0x7f, 0, step, 0x40);
            func_020765d0(0, scale, scale, (void *)func_ov004_020d6be0, 0);
            func_02076504(1, 0, 1, 0);
            func_02076b60(1);
        } else {
            func_02074860(param_1->w38, param_1->w44 * size * 2);
            func_02074860(param_1->w3c, param_1->w44 * param_1->wf4 * 2);
            func_02076640(3, 0);
            func_020766c8(0, 1, param_1->w38, 1, 0, (param_1->w44 * param_1->wf4) >> 1, 0x7f, 0, step, 0);
            func_020766c8(1, 1, param_1->w3c, 1, 0, (param_1->w44 * param_1->wf4) >> 1, 0x7f, 0, step, 0x7f);
            func_020765d0(0, scale, scale, (void *)func_ov004_020d6be0, 0);
            func_02076504(3, 0, 1, 0);
            func_02076b60(1);
        }
    }
}
