extern char data_020b18a0[];
extern int func_020055e0(char *object);
extern int func_0206c614(char *object);
extern void func_020055f0(char *a, int b);
int func_0200551c(int param_1)
{
    char *base;
    char *slot;
    int i;
    base = *(char **)(data_020b18a0 + 0x10) + 0x3d28;
    slot = base + param_1 * 0x3a4;
    if (func_020055e0(slot) != 0) {
        if (func_0206c614(slot) == 0) {
            return -3;
        }
    }
    *(short *)(slot + 0x4a) = -1;
    *(unsigned int *)(slot + 0x58) = 0;
    *(unsigned int *)(slot + 0x5c) = 0;
    *(unsigned char *)(slot + 0x61) = 0;
    *(unsigned char *)(slot + 0x62) = 0;
    *(unsigned char *)(slot + 0x63) = 0;
    *(unsigned int *)(slot + 0x50) = 0;
    *(unsigned int *)(slot + 0x54) = 0;
    *(unsigned int *)(slot + 0xe8) &= 0xffffffe1;
    *(unsigned char *)(slot + 0x60) = 0;
    *(unsigned char *)(slot + 0x48) = 0;
    *(unsigned int *)(slot + 0x64) = 0;
    *(unsigned int *)(slot + 0xec) = 9;
    *(unsigned char *)(slot + 0xf0) = 5;
    *(unsigned char *)(slot + 0x7d) = 8;
    *(unsigned int *)(slot + 0x70) = 0;
    *(unsigned int *)(slot + 0x74) = 0;
    *(unsigned int *)(slot + 0x78) = 0;
    *(unsigned char *)(slot + 0x7c) = 0;
    *(unsigned int *)(slot + 0x80) = 0;
    *(unsigned int *)(slot + 0x84) = 3;
    for (i = 0; i < 2; i = i + 1) {
        func_020055f0(slot + 0x68 + i * 4, 4);
    }
    return 0;
}
