extern unsigned int * func_020685e0();
extern int func_0206896c();

void func_0201e144(int param_1,int param_2,int param_3,unsigned int param_4)

{
    unsigned int *puVar1;

    *(int *)(param_1 + 0x28) = param_2;
    *(int *)(param_1 + 0x2c) = param_3;
    puVar1 = func_020685e0(param_2,param_3,0);
    *(unsigned int **)(param_1 + 4) = puVar1;
    if (puVar1 != (unsigned int *)0x0) {
        func_0206896c((unsigned char *)(param_1 + 8),(unsigned int)puVar1,param_4);
        func_0206896c((unsigned char *)(param_1 + 0x18),*(unsigned int *)(param_1 + 4),-param_4);
    }
    return;
}
