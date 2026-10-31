extern void *func_02008444();
extern int func_0200f330();
extern int func_0200f320();
extern void *func_02010b7c();
extern unsigned int func_02015188();
extern int func_0202cb64();
extern int func_02057718();

unsigned int func_020454f4(int param_1, int param_2, int param_3)
{
    void *puVar1;
    int iVar2;

    puVar1 = func_02008444();
    iVar2 = func_0200f330(puVar1);
    if (iVar2 != 0) {
        return 0;
    }
    if (*(char *)(param_1 + 0x119) != 0) {
        return 0;
    }
    if (((((unsigned int)*(unsigned char *)(param_1 + 0xe0)) << 0x1d) >> 0x1f) != 0) {
        return 0;
    }
    puVar1 = func_02008444();
    iVar2 = func_0200f320(puVar1);
    if (iVar2 != 0) {
        return 0;
    }
    if (func_02015188(func_02010b7c()) == 0) {
        return 0;
    }
    if (func_0202cb64(func_02010b7c()) != 0) {
        return 0;
    }
    return func_02057718(param_1, param_2, param_3);
}
