extern unsigned int * func_0201003c();
extern int func_0203cc20();
extern int func_0203d494();
extern int func_020521d4();

void func_0203d44c(int param_1)

{
    int iVar1;
    unsigned int *puVar2;

    func_0203cc20(param_1);
    puVar2 = func_0201003c();
    func_0203d494((unsigned char *)puVar2);
    iVar1 = ((int)0x0209ab04u);
    if (((*(char *)(param_1 + 4) != '\0') && (*(char *)(param_1 + 4) != '\n')) &&
          (*(char *)(param_1 + 0x50) == '\0')) {
        func_020521d4(((int)0x0209ab04u),0,6,0x100);
        func_020521d4(iVar1,1,6,0x100);
    }
    return;
}
