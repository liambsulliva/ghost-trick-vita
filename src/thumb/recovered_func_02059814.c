extern int func_02008444();
extern int func_0200f330();
extern int func_0202ff58();

unsigned int func_02059814(void)

{
    int iVar1;

    iVar1 = func_0202ff58();
    if (iVar1 != 0) {
        func_02008444();
        iVar1 = func_0200f330();
        if (iVar1 == 0) {
            return 1;
        }
    }
    return 0;
}
