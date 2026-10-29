extern void func_02008378();
extern int func_020083b0();
extern int func_020083c0();
extern int func_0201f0a0();
extern int func_020349b0();
extern int func_020452e8();
extern int func_020615e8();

void func_0206167c(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    int iVar1;
    int iVar2;
    unsigned char auStack_24 [12];
    unsigned char auStack_30 [12];
    unsigned char auStack_3c [12];
    unsigned char auStack_48 [12];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_020083c0(auStack_24,param_1 + 0x98,*(unsigned int *)(param_1 + 0xd0));
    iVar1 = func_020452e8(auStack_24);
    iVar2 = func_020615e8(param_1);
    if (iVar1 > iVar2) {
        func_0201f0a0(auStack_30,auStack_24);
        func_020349b0(auStack_48,auStack_30,iVar2);
        func_02008378(auStack_3c,*(unsigned int *)(param_1 + 0xd0),auStack_48);
        func_020083b0(param_1 + 0x98,auStack_3c);
    }
    return;
}
