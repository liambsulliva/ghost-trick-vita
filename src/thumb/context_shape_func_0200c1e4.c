extern int func_0200b3fc();
extern int func_0206e5d4();

void func_0200c1e4(unsigned int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned char auStack_20 [16];
    unsigned int uStack_10;

    if ((param_2 & 1) != 0) {
        uVar2 = ((unsigned int)0xfffff000u);
    }
    else {
        uVar2 = 0x1000;
    }
    if ((param_2 & 2) != 0) {
        uVar1 = ((unsigned int)0xfffff000u);
    }
    else {
        uVar1 = 0x1000;
    }
    uStack_10 = param_4;
    func_0200b3fc(auStack_20,uVar2,uVar1);
    func_0206e5d4(param_1,auStack_20,param_1);
    return;
}
