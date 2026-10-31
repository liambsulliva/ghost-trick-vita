extern unsigned char data_020cff60[];
extern unsigned int *func_02008444(void);
extern void func_0200ef08(unsigned int *object);
extern void func_0202665c(void *object);
extern unsigned int func_0205da98(int param_1);

void func_0205e10c(char *obj)
{
    int x;

    switch (*(unsigned char *)(obj + 0x4c)) {
    case 0:
        func_0200ef08(func_02008444());
        *(unsigned char *)(obj + 0x4c) = 2;
        break;
    case 1:
        break;
    case 2:
        *(short *)(obj + 0x4a) = *(short *)(obj + 0x4a) - (short)(data_020cff60[1] + 1);
        if (*(short *)(obj + 0x4a) < -0xf) {
            *(short *)(obj + 0x4a) = -0xf;
        }
        x = *(int *)(obj + 0x58) + *(int *)(obj + 0x5c);
        *(int *)(obj + 0x5c) = *(int *)(obj + 0x5c) + *(int *)(obj + 0x60);
        *(int *)(obj + 0x58) = x;
        *(int *)(obj + 0x64) = *(int *)(obj + 0x64) + *(int *)(obj + 0x68);
        *(int *)(obj + 0x68) = *(int *)(obj + 0x68) + *(int *)(obj + 0x6c);
        if (x < 0) {
            *(int *)(obj + 0x58) = 0;
        }
        if (*(int *)(obj + 0x64) < 0) {
            *(int *)(obj + 0x64) = 0;
            *(int *)(obj + 0x68) = 0;
            *(int *)(obj + 0x6c) = 0;
        }
        break;
    }
    if (*(int *)(obj + 0x7c) != 0 && func_0205da98(*(int *)(obj + 0x7c)) != 0 && *(unsigned char *)(obj + 0x54) != 0) {
        func_0202665c(func_02008444());
        *(unsigned char *)(obj + 0x54) = 0;
    }
}
