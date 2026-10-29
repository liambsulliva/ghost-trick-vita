extern int func_02008444();
extern int func_02011320();

unsigned int func_02032664(void)

{
    int iVar1;

    func_02008444();
    iVar1 = func_02011320();
    if (iVar1 == 3) {
        return 1;
    }
    return 0;
}
