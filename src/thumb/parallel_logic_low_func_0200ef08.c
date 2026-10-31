extern unsigned char *func_02009c1c();
extern int func_0202f3a4();
extern int func_0200a2f8();

void func_0200ef08(int param_1)
{
    unsigned int uVar1;
    unsigned char *puVar2;
    unsigned char value;

    if (*(int *)(param_1 + 0xee0) != 0) {
        if ((((unsigned int)*(int *)(param_1 + 8)) << 16) >> 31 == 0) {
            uVar1 = ((*(unsigned int *)(param_1 + 0xc)) << 17) >> 30;
            value = 4;
            if (uVar1 == 1) {
                value = 5;
            }
            else if (uVar1 == 2) {
                value = 8;
            }
            puVar2 = func_02009c1c();
            func_0202f3a4(puVar2, 0x10, value);
            func_0200a2f8(*(int *)(param_1 + 0xee0), *(int *)(param_1 + 0xdd4));
        }
        *(unsigned int *)(param_1 + 0xc) = *(unsigned int *)(param_1 + 0xc) | 0x1000;
    }
    return;
}
