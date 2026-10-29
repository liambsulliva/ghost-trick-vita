extern int func_0200af44();
extern int func_0200b074();
extern int func_0200b784();
extern int func_0200b794();
extern int func_0200b848();

void func_0200bb8c(int param_1,unsigned int param_2)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = func_0200b848(param_1 + 0x14);
    if (iVar1 != 0) {
        func_0200b074();
        func_0200af44(iVar1,param_2,*(unsigned char *)(param_1 + 10));
        uVar2 = func_0200b794(iVar1);
        func_0200b784(param_1 + 0x14,uVar2);
    }
    return;
}
