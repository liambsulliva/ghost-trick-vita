extern char data_020cee88;
extern int func_02008828();
extern int func_0200daac();
extern int func_0200eab4();
extern int func_0200eab8();
extern int func_0200eac4();
extern int func_02027c28();
extern int func_0202fbcc();
extern int func_0200eabc();
extern int func_02051508();
extern int func_020790c8();

void func_020275cc(unsigned int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    unsigned int uVar4;
    int iVar5;
    unsigned char auStack_28 [16];
    unsigned int uStack_18;

    uStack_18 = param_4;
    uVar2 = func_02027c28(param_2);
    func_0202fbcc(auStack_28,5);
    uVar1 = ((unsigned int)&data_020cee88);
    while (iVar5 = func_0200eac4(auStack_28), iVar5 != 0) {
        uVar3 = func_0200eab4(auStack_28);
        iVar5 = func_02008828(uVar3,uVar1);
        if (iVar5 != 0) {
            uVar3 = func_0200eab8(auStack_28);
            uVar4 = func_0200daac();
            iVar5 = func_020790c8(uVar4,uVar2);
            if (iVar5 != 0) {
                func_02051508(uVar3,param_3);
            }
        }
        func_0200eabc(auStack_28,0);
    }
    return;
}
