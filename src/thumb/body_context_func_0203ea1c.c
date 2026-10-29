extern int func_02008444();
extern int func_0200ec24();
extern int func_0200fe74();
extern int func_02011404();
extern int func_02022f6c();

void func_0203ea1c(int param_1)

{
    unsigned int uVar1;

    if (*(char *)(param_1 + 4) == '\0') {
        func_02008444();
        func_0200fe74();
        uVar1 = func_02008444();
        func_0200ec24(uVar1,0);
        func_02008444();
        func_02011404();
        *(signed char *)(param_1 + 4) = *(char *)(param_1 + 4) + '\x01';
        return;
    }
    func_02022f6c(param_1);
    return;
}
