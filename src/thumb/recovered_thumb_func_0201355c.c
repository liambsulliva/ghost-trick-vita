extern char data_02084130;
extern char data_02084144;
extern int func_02008444();
extern int func_02010384();
extern int func_02010f0c();
extern int func_0201123c();
extern int func_02011264();
extern int func_02011598();
extern int func_02013514();
extern int func_020135b8();
extern int func_02022f34();
extern int func_020790c8();

void func_0201355c(unsigned int param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned int uVar1;
    unsigned int uVar2;
    int iVar3;

    uVar1 = func_02013514(param_3);
    func_02010384(param_2,1);
    uVar2 = func_02011264();
    uVar2 = func_0201123c(uVar2,0);
    func_02022f34(uVar2,((unsigned int)&data_02084130),0);
    func_02008444();
    func_02011598();
    uVar2 = func_02008444();
    func_02010f0c(uVar2,2,2);
    iVar3 = func_020790c8(uVar1,((unsigned int)&data_02084144));
    if (iVar3 == 0) {
        func_020135b8(param_2,1);
    }
    return;
}
