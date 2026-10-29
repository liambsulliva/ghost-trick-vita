extern char data_02083e40;
struct State {
    unsigned int before_flags[21];
    unsigned int flags;
};
extern struct State data_020c9e48;
extern char data_020ce39c;
extern int func_02009900();
extern int func_0201123c();
extern int func_02011264();
extern void func_02011448();
extern int func_02022fa4();
extern int func_0202fad4();

void func_02011404(void)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02011264();
    uVar1 = func_0201123c(uVar1,1);
    iVar2 = func_02022fa4(uVar1,((unsigned int)&data_02083e40));
    if (iVar2 == 0) {
        data_020c9e48.flags = ((unsigned int)0xfffff7ffu) & data_020c9e48.flags;
        uVar1 = func_02009900();
        func_0202fad4(uVar1,((unsigned int)&data_020ce39c));
        return;
    }
    func_02011448();
    return;
}
