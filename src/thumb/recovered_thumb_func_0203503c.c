extern int func_02008444();
extern int func_0201192c();
extern int func_020141f4();
extern int func_02022dd8();
extern int func_02031b30();

void func_0203503c(int param_1)

{
    unsigned int uVar1;

    uVar1 = func_02008444();
    func_0201192c(uVar1,3);
    uVar1 = func_02008444();
    func_02031b30(uVar1,1);
    func_02008444();
    func_020141f4();
    uVar1 = func_02022dd8(param_1);
    *(unsigned int *)(param_1 + 0x20) = uVar1;
    *(unsigned char *)(param_1 + 4) = 0;
    if (*(int *)(param_1 + 8) == 1) {
        *(unsigned char *)(param_1 + 0x3e) = 1;
    }
    return;
}
