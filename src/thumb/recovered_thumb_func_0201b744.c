extern int func_02009c1c();
extern int func_0202f1bc();

void func_0201b744(int param_1)

{
    unsigned int uVar1;
    int iVar2;

    iVar2 = *(int *)(param_1 + 0x6c);
    if (((iVar2 != 0) && (*(int *)(param_1 + 0x70) != 0)) && (*(char *)(param_1 + 0x23) == '\0')) {
        if (*(char *)(param_1 + 0x22) == '\x01') {
            uVar1 = func_02009c1c();
            func_0202f1bc(uVar1,iVar2,0x2000,*(unsigned int *)(param_1 + 0x78),0x17);
            uVar1 = func_02009c1c();
            func_0202f1bc(uVar1,*(unsigned int *)(param_1 + 0x70),0,*(unsigned int *)(param_1 + 0x7c),0x1b);
            return;
        }
        uVar1 = func_02009c1c();
        func_0202f1bc(uVar1,iVar2,0x2000,*(unsigned int *)(param_1 + 0x78),9);
        uVar1 = func_02009c1c();
        func_0202f1bc(uVar1,*(unsigned int *)(param_1 + 0x70),0,*(unsigned int *)(param_1 + 0x7c),0xd);
    }
    return;
}
