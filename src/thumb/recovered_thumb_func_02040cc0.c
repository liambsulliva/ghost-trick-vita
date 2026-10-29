extern char data_0209b162;
extern char data_0209b192;
extern char data_0209b1dc;
extern char data_020ce3f0;
extern int func_02009900();
extern int func_0202f7e8();
extern int func_02040318();
extern int func_0204050c();
extern int func_02040774();
extern int func_020407f4();

void func_02040cc0(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    func_0204050c();
    uVar2 = ((unsigned int)&data_0209b1dc);
    *(unsigned int *)(param_1 + 0x50) = 0;
    func_02040774(param_1,uVar2);
    iVar1 = func_02040318(((unsigned int)&data_020ce3f0));
    *(int *)(param_1 + 0x50) = iVar1;
    if (iVar1 != 0) {
        uVar2 = func_02009900();
        func_0202f7e8(uVar2,iVar1,10,0);
    }
    if (*(char *)(param_1 + 0x2c) == '\0') {
        func_020407f4(param_1,((unsigned int)&data_0209b192));
        return;
    }
    func_020407f4(param_1,((unsigned int)&data_0209b162));
    return;
}
