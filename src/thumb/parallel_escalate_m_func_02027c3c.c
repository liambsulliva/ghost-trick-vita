extern char data_020cf898;
extern int func_02008828();
extern int func_0200eab4();
extern int func_0200eab8();
extern int func_0200eac4();
extern int func_0202fbcc();
extern int func_0200eabc();
extern int func_02014220();
extern int func_020790c8();

unsigned int func_02027c3c(unsigned int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int uVar1;
    unsigned int uVar3;
    unsigned int uVar4;
    int iVar5;
    unsigned char auStack_28 [16];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_0202fbcc(auStack_28,6);
    uVar1 = ((unsigned int)&data_020cf898);
    while (iVar5 = func_0200eac4(auStack_28), iVar5 != 0) {
        uVar3 = func_0200eab4(auStack_28);
        iVar5 = func_02008828(uVar3,uVar1);
        if (iVar5 != 0) {
            uVar3 = func_0200eab8(auStack_28);
            uVar4 = func_02014220(uVar3);
            iVar5 = func_020790c8(param_1,uVar4);
            if (iVar5 == 0) {
                return uVar3;
            }
        }
        func_0200eabc(auStack_28,0);
    }
    return 0;
}
