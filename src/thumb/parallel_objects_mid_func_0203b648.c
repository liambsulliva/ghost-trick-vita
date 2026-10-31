extern unsigned int *func_02008444();
extern int func_02010b38();
extern int func_0200f084();
extern int func_0200e930();
extern int func_02010d18();
extern unsigned int func_0203b69c();

unsigned int func_0203b648(unsigned char *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4)
{
    unsigned int *puVar1;
    unsigned int uVar3;

    puVar1 = func_02008444();
    if (func_02010b38((int)puVar1) != 0) {
        return 0;
    }
    puVar1 = func_02008444();
    func_0200f084((int)puVar1, 1, *(int *)(param_1 + 8) == 0, 1, 1);
    puVar1 = func_02008444();
    uVar3 = func_0203b69c((int)puVar1);
    *(unsigned int *)(param_1 + 0x34) = uVar3;
    if (uVar3 != 0) {
        puVar1 = func_02008444();
        func_0200e930((int)puVar1, 0);
    }
    puVar1 = func_02008444();
    func_02010d18((int)puVar1);
    *(unsigned int *)(param_1 + 0x28) = 0;
    return 1;
}
