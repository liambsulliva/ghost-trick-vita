extern void func_0200bd78();
extern void func_0207487c();
extern void func_02070290();
extern void func_020702f4();

void func_0200bcd0(unsigned int param_1)
{
    unsigned int i;
    unsigned int n;

    if ((2 & *(unsigned char *)(param_1 + 0xa83)) != 0) {
        switch (*(unsigned char *)(param_1 + 0xa84)) {
        case 0:
            for (i = (unsigned int)*(unsigned char *)(param_1 + 0xa81) >> 2; i < *(unsigned char *)(param_1 + 0xa82); i++) {
                func_0200bd78(param_1 + i * 0x20, param_1 + 0x400 + i * 0x10);
            }
            func_0207487c(param_1, 0x400);
            n = *(unsigned char *)(param_1 + 0xa81);
            func_02070290(param_1 + n * 8, n * 8, (0x80 - n) * 8);
            break;
        case 1:
            for (i = (unsigned int)*(unsigned char *)(param_1 + 0xa81) >> 2; i < *(unsigned char *)(param_1 + 0xa82); i++) {
                func_0200bd78(param_1 + i * 0x20, param_1 + 0x400 + i * 0x10);
            }
            func_0207487c(param_1, 0x400);
            n = *(unsigned char *)(param_1 + 0xa81);
            func_020702f4(param_1 + n * 8, n * 8, (0x80 - n) * 8);
            break;
        }
    }
}
