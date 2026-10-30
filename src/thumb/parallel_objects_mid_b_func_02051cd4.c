extern char data_020cef40;
extern char data_020ceec0;
extern unsigned int * func_02008444();
extern int func_02010298();
extern int func_02051dc4();
extern int func_020102a4();
extern char * func_02051d44();
extern int func_02027bec();
extern int func_02027c98();
extern int func_02010300();
extern int func_02078e84();

void func_02051cd4(char *param_1)
{
    unsigned int *puVar1;
    char *puVar2;
    int iVar3;

    func_02078e84(&data_020cef40, param_1);
    puVar1 = func_02008444();
    func_02010298(puVar1, param_1);
    func_02051dc4(param_1, 0);
    puVar1 = func_02008444();
    func_020102a4(puVar1, 0);
    puVar2 = func_02051d44(&data_020ceec0);
    if (puVar2 != 0) {
        puVar1 = func_02008444();
        func_020102a4(puVar1, puVar2);
        func_02027bec(puVar2);
        func_02027c98(puVar2, 0);
    }
    func_02051dc4(&data_020cef40, 1);
    iVar3 = (int)func_02051d44(&data_020ceec0);
    puVar1 = func_02008444();
    func_02010300(puVar1, iVar3);
}
