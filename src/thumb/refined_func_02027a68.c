extern char data_020cf8d0;
extern int func_02008398();
extern int func_02008828();
extern int func_0200eab4();
extern int func_0200eab8();
extern int func_0200eac4();
extern int func_020265c4();
extern int func_02027b08();
extern int func_02027c28();
extern int func_0202fbb0();
extern int func_0200eabc();
extern int func_02059f3c();
extern int func_020790c8();

void func_02027a68(int param_1,unsigned int param_2)

{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    unsigned int uVar4;
    int iVar5;
    unsigned char auStack_24 [16];
    unsigned char auStack_30 [12];

    uVar1 = func_02027c28(*(unsigned short *)(param_1 + 2));
    func_0202fbb0(auStack_24);
    while (iVar5 = func_0200eac4(auStack_24), iVar5 != 0) {
        uVar2 = func_0200eab4(auStack_24);
        iVar5 = func_02008828(uVar2,((unsigned int)&data_020cf8d0));
        if (iVar5 != 0) {
            uVar2 = func_0200eab8(auStack_24);
            uVar3 = func_02027b08();
            iVar5 = func_020790c8(uVar1,uVar3);
            if (iVar5 == 0) {
                uVar3 = func_020265c4(param_1 + 4);
                uVar4 = func_020265c4(param_1 + 8);
                func_02008398(auStack_30,uVar3,uVar4,0);
                uVar3 = func_020265c4(param_1 + 0xc);
                uVar4 = func_020265c4(param_1 + 0x10);
                func_02059f3c(uVar2,auStack_30,uVar3,uVar4,param_2);
            }
        }
        func_0200eabc(auStack_24,0);
    }
    return;
}
