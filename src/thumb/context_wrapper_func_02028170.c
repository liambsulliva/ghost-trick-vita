extern signed char data_020cee88;
extern int func_02008828();
extern int func_0200daac();
extern int func_0200eab4();
extern int func_0200eab8();
extern int func_0200eac4();
extern int func_020266d4();
extern int func_02028148();
extern int func_02028160();
extern int func_0202fbcc();
extern int func_0200eabc();
extern int func_020790c8();

unsigned int func_02028170(unsigned int param_1,unsigned int param_2,int param_3,unsigned int param_4)

{
    signed char *pcVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    unsigned int uVar4;
    int iVar5;
    unsigned char auStack_28 [16];
    unsigned int uStack_18;

    uStack_18 = param_4;
    pcVar1 = (signed char *)func_02028160();
    if (*pcVar1 == '?') {
        uVar2 = func_02028148(param_1);
    }
    else {
        func_0202fbcc(auStack_28,5);
        uVar2 = ((unsigned int)&data_020cee88);
        while (iVar5 = func_0200eac4(auStack_28), iVar5 != 0) {
            uVar3 = func_0200eab4(auStack_28);
            iVar5 = func_02008828(uVar3,uVar2);
            if (iVar5 != 0) {
                uVar3 = func_0200eab8(auStack_28);
                uVar4 = func_0200daac();
                iVar5 = func_020790c8(pcVar1,uVar4);
                if (iVar5 == 0) {
                    if (param_3 == 0) {
                        uVar3 = func_020266d4(uVar3);
                    }
                    return uVar3;
                }
            }
            func_0200eabc(auStack_28,0);
        }
        uVar2 = 0;
    }
    return uVar2;
}
