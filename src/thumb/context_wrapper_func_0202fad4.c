typedef int code();
extern int func_02008828();
extern int func_0200eab4();
extern int func_0200eac4();
extern int func_0200fe48();
extern int func_0202fb38();
extern int func_0202fbb0();
extern int func_0200eabc();

void func_0202fad4(unsigned int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int uVar1;
    int *piVar2;
    int iVar3;
    unsigned char auStack_28 [16];
    unsigned int uStack_18;

    uStack_18 = param_4;
    func_0202fbb0(auStack_28);
    while (iVar3 = func_0200eac4(auStack_28), iVar3 != 0) {
        func_0200eab4(auStack_28);
        iVar3 = func_0200fe48();
        if (iVar3 == 0) {
            func_0200eab4(auStack_28);
            iVar3 = func_0202fb38();
            if (iVar3 == 0) {
                uVar1 = func_0200eab4(auStack_28);
                iVar3 = func_02008828(uVar1,param_2);
                if (iVar3 != 0) {
                    piVar2 = (int *)func_0200eab4(auStack_28);
                    (**(code **)(*piVar2 + 0x20))(piVar2,0);
                }
            }
        }
        func_0200eabc(auStack_28,0);
    }
    return;
}
