struct Panel {
    unsigned char pad000[0xaa0];
    unsigned int waa0;
    int waa4;
    unsigned char paa8;
    unsigned char low:3;
    unsigned char bit8:1;
    unsigned char bit10:1;
    unsigned char high:3;
    unsigned char mode:3;
};

extern unsigned int *func_02008444(void);
extern unsigned int func_ov004_020d58a4(unsigned int *a);
extern void func_ov004_020d58b0(struct Panel *a, int b, int c, unsigned int d);

void func_ov004_020d5854(struct Panel *param_1, int param_2, int param_3, unsigned int param_4)
{
    if (param_1->mode == 0) {
        param_1->bit8 = 1;
        param_1->bit10 = 1;
        param_1->waa0 = func_ov004_020d58a4(func_02008444());
        param_1->waa4 = 0;
        func_ov004_020d58b0(param_1, param_2, param_3, param_4);
    }
}
