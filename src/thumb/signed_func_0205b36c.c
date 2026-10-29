extern unsigned int * func_02008444();
extern int func_02029910();
extern int func_0205b020();
extern int func_0205b3a4();

void func_0205b36c(int param_1,int param_2)

{
    unsigned int *puVar1;
    unsigned char *puVar2;

    if (param_2 == *(signed char *)(param_1 + 0x120)) {
        puVar1 = func_02008444();
        puVar2 = (unsigned char *)func_0205b020((unsigned char *)puVar1,param_2);
        if (puVar2 != (unsigned char *)0x0) {
            func_02029910(puVar2);
            return;
        }
    }
    puVar1 = func_02008444();
    puVar2 = (unsigned char *)func_0205b3a4((unsigned char *)puVar1,param_2);
    if (puVar2 != (unsigned char *)0x0) {
        func_02029910(puVar2);
    }
    return;
}
