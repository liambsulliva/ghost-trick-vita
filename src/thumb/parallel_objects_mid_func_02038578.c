extern unsigned int func_02037130();
extern void func_0201fd60();

void func_02038578(int param_1)
{
    unsigned int uVar1;

    uVar1 = func_02037130((unsigned char)*(int *)(param_1 + 0x20));
    if (uVar1 != 0xffffffff) {
        *(unsigned char *)(param_1 + 0x33) = 1;
        *(int *)(param_1 + 0x28) = 1;
        *(unsigned char *)(param_1 + 0x30) = 0;
        *(signed char *)(param_1 + 0x31) = *(signed char *)(param_1 + 0x30);
        func_0201fd60(*(int *)(param_1 + 0x14), uVar1);
    }
    return;
}
