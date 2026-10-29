extern unsigned char * func_0201123c();
extern unsigned char * func_02011264();
extern int func_02022f34();

void func_0203e45c(int param_1)

{
    unsigned char *puVar1;

    if (*(char *)(param_1 + 0x20) != '\0') {
        puVar1 = func_02011264();
        puVar1 = func_0201123c(puVar1,0);
        func_02022f34((int)puVar1,((int)0x0209acf0u),0);
        return;
    }
    puVar1 = func_02011264();
    puVar1 = func_0201123c(puVar1,0);
    func_02022f34((int)puVar1,((int)0x0209ad0cu),0);
    return;
}
