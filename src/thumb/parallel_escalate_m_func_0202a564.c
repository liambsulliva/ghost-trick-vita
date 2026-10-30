extern int func_02006f18(void);
extern int func_02006f34(void);
extern void func_02006ff0(void);
extern void func_0200700c(void);
extern int func_02029d4c(int object, int value, int mode);

int func_0202a564(int object, int id)
{
    unsigned short *rec;
    int i;

    rec = (unsigned short *)(object + 0x1c8);
    if (*(unsigned char *)(object + 0x24) == 2) {
        return 0;
    }
    if (*(unsigned short *)(object + 0x2c) >= 1) {
        return 0;
    }
    for (i = 0; i < 2; i++) {
        if (i >= 1) {
            return 0;
        }
        if (*rec == 0xffff) {
            *rec = id;
            break;
        }
        rec = rec + 6;
    }
    *(int *)(rec + 2) = func_02029d4c(object, func_02006f18(), 1);
    func_02006ff0();
    *(int *)(rec + 4) = func_02029d4c(object, func_02006f34(), 1);
    func_0200700c();
    *(unsigned short *)(object + 0x2c) = *(unsigned short *)(object + 0x2c) + 1;
    return 1;
}
