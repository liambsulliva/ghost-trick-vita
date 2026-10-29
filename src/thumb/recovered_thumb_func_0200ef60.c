extern int func_02009c1c();
extern int func_0200a2f8();
extern int func_0202f3a4();

void func_0200ef60(int param_1)

{
    unsigned int uVar1;

    if (*(int *)(param_1 + 0xee0) != 0) {
        uVar1 = func_02009c1c();
        func_0202f3a4(uVar1,0x10,0);
        func_0200a2f8(*(unsigned int *)(param_1 + 0xee0),*(unsigned int *)(param_1 + ((int)0x00000dd4u)));
        *(unsigned int *)(param_1 + 0xc) = ((unsigned int)0xffffefffu) & *(unsigned int *)(param_1 + 0xc);
    }
    return;
}
