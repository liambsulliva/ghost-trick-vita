extern const char data_0209b404[];
extern const char data_0209b40c[];
extern const char data_0209b420[];
extern const char data_0209b434[];
extern unsigned int data_020c9e48;
extern int func_020300ec();
extern int func_02030298();
extern int func_02041c88();
extern int func_020521d4();
extern int func_0206c440();
extern int func_0206c614();
extern int func_0206c704();
extern int func_0206c7d0();

void func_02041bc0(void)
{
    int iVar1;
    char acStack_78 [32];
    char acStack_98 [32];
    unsigned int auStack_58 [18];

    func_0206c440(auStack_58);
    iVar1 = (func_0206c7d0(auStack_58,data_0209b404) != 0);
    if (iVar1 != 0) {
        func_0206c704((char *)auStack_58,acStack_98,0x20);
        func_0206c614((char *)auStack_58);
        func_020300ec();
        iVar1 = func_02041c88(acStack_98,acStack_78);
        if (iVar1 != 0) {
            func_02030298(5,0xb,0,data_0209b40c);
        }
        func_02030298(5,0xc,0,acStack_78);
        data_020c9e48 = 0xffffe3ffu & data_020c9e48;
        func_02030298(5,0x16,0,data_0209b420);
    }
    func_020521d4(data_0209b434,0,0x1e,0x100);
    return;
}
