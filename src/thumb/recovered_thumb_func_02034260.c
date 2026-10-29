extern char data_0209a1d4;
extern char data_020c9d50;
extern char data_020c9e88;
extern int func_02008444();
extern int func_0200f330();
extern int func_02010384();
extern int func_02010b38();
extern int func_02010ef4();
extern int func_02010f0c();
extern int func_02010fbc();
extern int func_02011244();
extern int func_02011598();
extern int func_0201192c();
extern int func_02013a14();
extern int func_0201572c();
extern int func_02022f34();
extern int func_02031b20();
extern int func_020322d4();
extern int func_0203435c();

void func_02034260(unsigned int param_1)

{
    int iVar1;
    unsigned int uVar2;

    func_02008444();
    iVar1 = func_020322d4();
    if (iVar1 != 0) {
        func_02008444();
        iVar1 = func_02013a14();
        if (iVar1 != 0) {
            func_02008444();
            iVar1 = func_0200f330();
            if (iVar1 == 0) {
                func_02008444();
                iVar1 = func_02011244();
                if (iVar1 == 0) {
                    func_02008444();
                    iVar1 = func_0203435c();
                    if (iVar1 == 0) {
                        uVar2 = func_02008444();
                        func_02010f0c(uVar2,1,1);
                    }
                }
            }
        }
    }
    if ((*(unsigned int *)(((int)&data_020c9d50) + 4) & 0x20000) != 0) {
        if ((*(short *)(((int)&data_020c9e88) + 0x12) == 0) && (iVar1 = func_0201572c(), iVar1 == 0)) {
            func_02008444();
            iVar1 = func_0203435c();
            if (iVar1 == 0) {
                func_02008444();
                iVar1 = func_020322d4();
                if (iVar1 != 0) {
                    func_02008444();
                    iVar1 = func_02013a14();
                    if (iVar1 != 0) {
                        uVar2 = func_02008444();
                        iVar1 = func_02010fbc(uVar2,1);
                        if (iVar1 != 0) {
                            func_02008444();
                            iVar1 = func_0200f330();
                            if (iVar1 == 0) {
                                func_02008444();
                                iVar1 = func_02010b38();
                                if (iVar1 == 0) {
                                    uVar2 = func_02008444();
                                    func_02010ef4(uVar2,1,0xffffffff);
                                    uVar2 = func_02031b20(param_1);
                                    func_02022f34(uVar2,((unsigned int)&data_0209a1d4),0);
                                    func_02008444();
                                    func_02011598();
                                    uVar2 = func_02008444();
                                    func_0201192c(uVar2,0x13);
                                    uVar2 = func_02008444();
                                    func_02010384(uVar2,1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return;
}
