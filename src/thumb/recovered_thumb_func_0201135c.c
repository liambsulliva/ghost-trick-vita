extern int func_0201123c();
extern int func_02011264();
extern int func_02022f74();

void func_0201135c(int param_1)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02011264();
    uVar1 = func_0201123c(uVar1,1);
    iVar2 = func_02022f74(uVar1,((unsigned int)0x020ce444u));
    if (iVar2 == 0) {
        uVar1 = func_02011264();
        uVar1 = func_0201123c(uVar1,1);
        iVar2 = func_02022f74(uVar1,((unsigned int)0x020ce1e8u));
        if (iVar2 == 0) {
            *(unsigned int *)(param_1 + 0xc) = *(unsigned int *)(param_1 + 0xc) | 0x1000000;
        }
    }
    return;
}
