extern unsigned char data_020ce320[];

extern char data_0209b2ac;
struct State {
    unsigned int before_flags[21];
    unsigned int flags;
};
extern struct State data_020c9e48;
extern char data_020cd96c;
extern char data_020ce364;
extern char data_020ce380;
extern char data_020ce39c;
extern char data_020ce428;
extern int func_020082f8();
extern int func_02008444();
extern int func_02008860();
extern int func_02009900();
extern int func_0200c82c();
extern int func_0201123c();
extern int func_02011264();
extern int func_0201572c();
extern int func_02022df0();
extern int func_02022fa4();
extern int func_0202f7e8();
extern int func_0202fad4();
extern int func_020322d4();
extern int func_02034a30();
extern int func_02040194();
extern int func_020401dc();
extern int func_02040204();

unsigned int func_02040354(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = func_0201572c();
    if (iVar1 != 0) {
        return 0;
    }
    data_020c9e48.flags = ((unsigned int)0xfffff7ffu) & data_020c9e48.flags;
    uVar2 = func_02009900();
    func_0202fad4(uVar2,((unsigned int)&data_020ce39c));
    uVar2 = func_02008860();
    func_0200c82c(uVar2,1);
    uVar2 = func_02011264();
    uVar2 = func_0201123c(uVar2,0);
    iVar1 = func_02022fa4(uVar2,((unsigned int)&data_0209b2ac));
    if ((((iVar1 != 0) && (iVar1 = func_02022df0(param_1), iVar1 != 0)) &&
            (iVar1 = func_020082f8(iVar1,((unsigned int)&data_020cd96c)), iVar1 != 0)) &&
          (iVar1 = func_02034a30(), iVar1 != 0)) {
        uVar2 = func_02040204(((unsigned int)&data_020ce428));
        *(unsigned int *)(param_1 + 0x20) = uVar2;
    }
    if (*(int *)(param_1 + 0x20) == 0) {
        func_02008444();
        iVar1 = func_020322d4();
        if (iVar1 != 0) {
            uVar2 = func_020401dc(((unsigned int)&data_020ce380));
        }
    else {
            uVar2 = func_02040194(((unsigned int)&data_020ce364));
        }
        *(unsigned int *)(param_1 + 0x20) = uVar2;
    }
    uVar2 = func_02009900();
    func_0202f7e8(uVar2,*(unsigned int *)(param_1 + 0x20),7,0);
    data_020ce320[(1) * 1] = 0;
    return 1;
}
