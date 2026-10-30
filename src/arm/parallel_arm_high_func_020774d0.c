extern void func_020768a8();
extern void func_020768bc();
void func_02074860(unsigned char *p,unsigned int n);

void func_020774d0(unsigned int *param_1)
{
    unsigned int *puVar1;
    unsigned int *puVar2;

    func_020768a8();
    puVar1 = (unsigned int *)param_1[6];
    if (puVar1 != (unsigned int *)0x0) {
        do {
            puVar2 = (unsigned int *)puVar1[1];
            puVar1[0] = 0;
            puVar1[1] = 0;
            func_02074860((unsigned char *)puVar1,8);
            puVar1 = puVar2;
        } while (puVar1 != (unsigned int *)0x0);
    }
    func_020768bc();
}
