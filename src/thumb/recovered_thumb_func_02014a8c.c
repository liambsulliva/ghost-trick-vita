extern char data_02084770;
extern char data_020b9c94;
extern char data_020cf93c;
extern int func_02008444();
extern int func_02009900();
extern int func_0200f910();
extern int func_0201003c();
extern int func_02010384();
extern int func_02010ef4();
extern int func_0201123c();
extern int func_02011264();
extern int func_020148b0();
extern int func_02014b3c();
extern int func_02015970();
extern int func_02015a34();
extern int func_02022f34();
extern int func_02022fa4();
extern int func_0202f7e8();
extern int func_0202fb48();

void func_02014a8c(int param_1,int param_2,int param_3,unsigned int param_4)

{
    unsigned int uVar1;
    int iVar2;
    unsigned int uVar3;

    if (param_2 == 0) {
        func_0201003c();
        func_02015a34();
        return;
    }
    uVar1 = func_02009900();
    iVar2 = func_0202fb48(uVar1,((unsigned int)&data_020cf93c),0xd);
    if (iVar2 != 0) {
        func_0200f910(iVar2,1);
    }
    uVar1 = func_02008444();
    func_02010384(uVar1,1);
    *(int *)(param_1 + 8) = param_2;
    func_0201003c();
    func_02015970();
    *(unsigned int *)(param_1 + 0xc) = 0;
    iVar2 = func_020148b0(((unsigned int)&data_020b9c94));
    *(int *)(param_1 + 4) = iVar2;
    if (iVar2 != 0) {
        func_02014b3c(iVar2,param_2,param_4);
        uVar1 = func_02009900();
        func_0202f7e8(uVar1,*(unsigned int *)(param_1 + 4),1,0);
    }
    if (param_3 == 0) {
        uVar1 = func_02011264();
        uVar3 = func_0201123c(uVar1,0);
        uVar1 = ((unsigned int)&data_02084770);
        iVar2 = func_02022fa4(uVar3,((unsigned int)&data_02084770));
        if (iVar2 == 0) {
            uVar3 = func_02011264();
            uVar3 = func_0201123c(uVar3,0);
            func_02022f34(uVar3,uVar1,0);
            uVar1 = func_02008444();
            func_02010ef4(uVar1,0xff,0xffffffff);
        }
    }
    return;
}
