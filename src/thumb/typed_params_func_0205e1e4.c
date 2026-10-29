extern unsigned int * func_02008444();
extern int func_0200ef60();
extern int func_020153d8();
extern int func_0202f600();
extern int func_0205e0d4();
extern int func_0205e0f8();

void func_0205e1e4(int param_1)

{
    unsigned int *puVar1;

    func_0202f600(param_1,0);
    if (*(char *)(param_1 + 0x53) == '\0') {
        puVar1 = func_02008444();
        func_0200ef60((int)puVar1);
        return;
    }
    func_020153d8(*(int *)(param_1 + 0x74),(unsigned char *)0x0);
    func_0205e0d4(*(unsigned char **)(param_1 + 0x74),1);
    func_020153d8(*(int *)(param_1 + 0x80),(unsigned char *)0x0);
    func_0205e0f8(*(unsigned char **)(param_1 + 0x80),1);
    return;
}
