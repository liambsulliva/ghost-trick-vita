extern char data_0209adec;
extern int func_02008444();
extern int func_02010384();
extern int func_0201123c();
extern int func_02011264();
extern int func_02011ca8();
extern int func_02011cdc();
extern int func_02022f34();
extern int func_02027518();
extern int func_02027528();

void func_0203e9d0(void)

{
    unsigned int uVar1;
    int iVar2;

    iVar2 = ((int)0x00001003u);
    func_02027528(((int)0x00001003u));
    uVar1 = func_02008444();
    func_02011cdc(uVar1,0);
    iVar2 = func_02027518(iVar2 + 7);
    if (iVar2 == 0) {
        func_02008444();
        func_02011ca8();
    }
    uVar1 = func_02011264();
    uVar1 = func_0201123c(uVar1,0);
    func_02022f34(uVar1,((unsigned int)&data_0209adec),0);
    uVar1 = func_02008444();
    func_02010384(uVar1,0);
    return;
}
