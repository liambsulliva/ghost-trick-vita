extern unsigned char *func_02008eb0(void);
extern unsigned int func_01ff80b8(unsigned char *object);

void func_0204eb98(char *param_1, int param_2)
{
    unsigned int value;
    int remainder;
    int offset;
    char *cell;

    offset = 0x106;
    value = func_01ff80b8(func_02008eb0());
    cell = param_1 + param_2;
    cell[offset] = (char)value;
    *(unsigned short *)(param_1 + param_2 * 2 + 0xc6) = 0;
    param_1 = param_1 + offset + 0x20;
    param_1[param_2] = 0;
    value = func_01ff80b8(func_02008eb0());
    remainder = value % 10;
    if (remainder < 6) {
        param_1[param_2] = 2;
        return;
    }
    if (remainder < 9) {
        param_1[param_2] = 1;
    }
}
