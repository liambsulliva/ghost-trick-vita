extern int func_02009900();
extern int func_0202f7e8();
extern int func_0205a778();

void func_02015b70(int param_1)

{
    unsigned int uVar1;

    if (*(int *)(param_1 + 0xc) == 0) {
        uVar1 = func_0205a778(((unsigned int)0x020cf980u));
        *(unsigned int *)(param_1 + 0xc) = uVar1;
        uVar1 = func_02009900();
        func_0202f7e8(uVar1,*(unsigned int *)(param_1 + 0xc),1,0);
    }
    return;
}
