extern char data_02083e84;
extern char data_02083e90;
extern char data_02083e9c;
extern char data_020cd7b4;
extern char data_020cf4d8;
extern int func_02009900();
extern int func_02010384();
extern int func_0201123c();
extern int func_02011264();
extern int func_0201192c();
extern int func_02012518();
extern int func_02022f34();
extern int func_02022f74();
extern int func_0202f7e8();
extern int func_02054d30();

void func_020118a8(int param_1,unsigned int param_2)

{
    unsigned int uVar1;
    int iVar2;
    unsigned int uVar3;

    *(unsigned int *)(param_1 + ((int)0x00000f14u)) = param_2;
    uVar1 = func_02011264();
    uVar1 = func_0201123c(uVar1,0);
    iVar2 = func_02022f74(uVar1,((unsigned int)&data_020cd7b4));
    if ((iVar2 == 0) || (iVar2 = func_02012518(param_1,((unsigned int)&data_02083e84),((unsigned int)&data_02083e90)), iVar2 != 0)) {
        uVar1 = func_02009900();
        uVar3 = func_02054d30(((unsigned int)&data_020cf4d8));
        func_0202f7e8(uVar1,uVar3,7,0);
        return;
    }
    func_02010384(param_1,1);
    func_0201192c(param_1,0x12);
    uVar1 = func_02011264();
    uVar1 = func_0201123c(uVar1,0);
    func_02022f34(uVar1,((unsigned int)&data_02083e9c),0);
    return;
}
