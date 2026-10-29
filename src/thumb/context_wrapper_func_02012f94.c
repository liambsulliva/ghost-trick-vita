extern int func_02008828();
extern int func_0200eab4();
extern int func_0200eac4();
extern int func_0202fbb0();
extern int func_0200eabc();

int func_02012f94(unsigned int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int uVar1;
    int iVar2;
    int iVar3;
    unsigned char auStack_28 [16];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_0202fbb0(auStack_28);
    iVar3 = 0;
    while (iVar2 = func_0200eac4(auStack_28), iVar2 != 0) {
        uVar1 = func_0200eab4(auStack_28);
        iVar2 = func_02008828(uVar1,param_2);
        if (iVar2 != 0) {
            iVar3 = iVar3 + 1;
        }
        func_0200eabc(auStack_28,0);
    }
    return iVar3;
}
