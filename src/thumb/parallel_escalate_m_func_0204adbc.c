extern void *func_02008444(void);
extern int func_0200f330(void *object);
extern void func_020471ec(unsigned int *out, char *object);
extern void func_020083b0(int *dst, int *src);

void func_0204adbc(char *param_1)
{
    short value;
    unsigned int i;
    unsigned int position[3];

    if (func_0200f330(func_02008444()) == 0) {
        *(short *)(param_1 + 0xc0) = *(short *)(param_1 + 0xc0) + *(short *)(param_1 + 0xc2);
        if (*(short *)(param_1 + 0xc0) < 0) {
            *(short *)(param_1 + 0xc0) = 0;
            *(short *)(param_1 + 0xc2) = 1;
        }
        if (*(short *)(param_1 + 0xc0) > 2) {
            *(short *)(param_1 + 0xc0) = 2;
            *(short *)(param_1 + 0xc2) = -1;
        }
        value = *(short *)(param_1 + 0xc0);
        if (value > 1) {
            value = 1;
        }
        *(short *)(param_1 + 0xc4) = value * 0x19 + 6;
        func_020471ec(position, param_1);
        i = 0;
        do {
            func_020083b0((int *)(*(int *)(param_1 + 0xbc) + i * 0xc), (int *)(*(int *)(param_1 + 0xbc) + (i + 1) * 0xc));
            i = i + 1;
        } while (i < 0x1f);
        func_020083b0((int *)(*(int *)(param_1 + 0xbc) + 0x174), (int *)position);
    }
}
