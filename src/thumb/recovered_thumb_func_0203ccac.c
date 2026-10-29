extern char data_0209ab34;
extern char data_0209ab40;
extern int func_02008444();
extern int func_0200847c();
extern int func_0200fe74();
extern int func_0201003c();
extern int func_0201123c();
extern int func_02011264();
extern int func_02011404();
extern int func_02022f34();
extern int func_02022f4c();
extern int func_0202a068();
extern int func_0202a16c();
extern int func_0203cd18();

void func_0203ccac(unsigned int param_1,int param_2)

{
    unsigned int uVar1;
    unsigned int uVar2;

    if (param_2 == 1) {
        uVar1 = func_02011264();
        uVar2 = func_0201123c(uVar1,0);
        uVar1 = ((unsigned int)&data_0209ab34);
    }
    else {
        uVar1 = func_02011264();
        uVar2 = func_0201123c(uVar1,0);
        uVar1 = ((unsigned int)&data_0209ab40);
    }
    func_02022f34(uVar2,uVar1,0);
    func_0201003c();
    func_0203cd18();
    func_02008444();
    func_0200fe74();
    func_02008444();
    func_02011404();
    uVar1 = func_02011264();
    func_0201123c(uVar1,1);
    func_02022f4c();
    uVar1 = func_0200847c();
    func_0202a068(uVar1,0);
    uVar1 = func_0200847c();
    func_0202a16c(uVar1,0);
    return;
}
