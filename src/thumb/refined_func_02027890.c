extern char data_020cf8ec;
extern int func_02008398();
extern int func_02008828();
extern int func_0200eab4();
extern int func_0200eab8();
extern int func_0200eac4();
extern int func_020265c4();
extern int func_0202792c();
extern int func_02027c28();
extern int func_0202fbb0();
extern int func_0200eabc();
extern int func_0205a458();
extern int func_020790c8();

void func_02027890(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    unsigned int uVar4;
    int iVar5;
    unsigned char auStack_28 [16];
    unsigned char auStack_34 [12];
    unsigned int uStack_18;

    uStack_18 = param_4;
    uVar1 = func_02027c28(*(unsigned short *)(param_1 + 2));
    func_0202fbb0(auStack_28);
    while (iVar5 = func_0200eac4(auStack_28), iVar5 != 0) {
        uVar2 = func_0200eab4(auStack_28);
        iVar5 = func_02008828(uVar2,((unsigned int)&data_020cf8ec));
        if (iVar5 != 0) {
            uVar2 = func_0200eab8(auStack_28);
            uVar3 = func_0202792c();
            iVar5 = func_020790c8(uVar1,uVar3);
            if (iVar5 == 0) {
                uVar3 = func_020265c4(param_1 + 4);
                uVar4 = func_020265c4(param_1 + 8);
                func_02008398(auStack_34,uVar3,uVar4,0);
                uVar3 = func_020265c4(param_1 + 0xc);
                uVar4 = func_020265c4(param_1 + 0x10);
                func_0205a458(uVar2,auStack_34,uVar3,uVar4);
            }
        }
        func_0200eabc(auStack_28,0);
    }
    return;
}
