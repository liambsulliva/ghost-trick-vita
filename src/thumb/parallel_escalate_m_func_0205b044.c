extern int func_02010b7c();
extern int func_02010b60();
extern unsigned int func_02010b64();
extern int func_0205b9e8();
extern int func_0205b0b0();
extern void func_020229b4();

signed char func_0205b044(char *param_1)
{
    int first;
    unsigned int second;
    unsigned int i;

    first = func_02010b60(func_02010b7c());
    second = func_02010b64(func_02010b7c());
    for (i = 0; i < *(unsigned short *)(param_1 + 0x28); i++) {
        if (((int *)(param_1 + 0x44))[i] != 0 && func_0205b9e8(((int *)(param_1 + 0x44))[i],first,second) != 0) {
            return (signed char)func_0205b0b0(((int *)(param_1 + 0x44))[i]);
        }
    }
    if (*(unsigned char *)(param_1 + 0x30) != 0) {
        if (second < 0x60) {
            func_020229b4(0x100);
        }
        else if (second >= 0x60 && second < 0xc0) {
            func_020229b4(0x200);
        }
    }
    return -1;
}
