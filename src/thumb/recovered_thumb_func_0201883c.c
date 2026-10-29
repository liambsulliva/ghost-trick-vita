extern int func_02018944();
extern int func_02018fa8();
extern int func_0201c130();

void func_0201883c(int param_1)

{
    int iVar1;

    if (*(int *)(param_1 + 0x54) != 1) {
        if (*(int *)(param_1 + 0x54) == 2) {
            *(char *)(param_1 + 0x4e) = *(char *)(param_1 + 0x4e) + '\x01';
            return;
        }
        if (*(char *)(param_1 + 0x5c) == '\0') {
            iVar1 = func_0201c130(*(unsigned short *)(param_1 + 0x28));
            if (iVar1 != 0) {
                func_02018fa8(param_1,*(unsigned short *)(param_1 + 0x28));
                *(unsigned short *)(param_1 + 0x28) = 0xff;
            }
            func_02018944(param_1);
            *(char *)(param_1 + 0x3b) = *(char *)(param_1 + 0x3b) + '\x01';
        }
    }
    return;
}
