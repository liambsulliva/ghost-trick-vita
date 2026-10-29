extern unsigned char * func_02010b7c();
extern int func_02013a3c();
extern int func_02013a54();

void func_02038090(int param_1,unsigned char param_2)

{
    unsigned char *puVar1;

    *(unsigned char *)(param_1 + 0x5a) = param_2;
    if (*(char *)(param_1 + 0x5a) != '\0') {
        puVar1 = func_02010b7c();
        func_02013a54(puVar1);
        return;
    }
    puVar1 = func_02010b7c();
    func_02013a3c(puVar1);
    return;
}
