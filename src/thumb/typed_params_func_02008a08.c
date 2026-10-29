extern int func_020201b4();
extern int func_02020d10();

void func_02008a08(int param_1,unsigned int *param_2)

{
    unsigned int *puVar1;

    puVar1 = (unsigned int *)func_02020d10();
    func_020201b4(puVar1,param_2);
    puVar1[0xc] = 0;
    puVar1[0xd] = 0;
    if (*(int *)(param_1 + 4) != 0) {
        *(unsigned int **)(*(int *)(param_1 + 4) + 0x30) = puVar1;
        puVar1[0xd] = *(unsigned int *)(param_1 + 4);
    }
    *(unsigned int **)(param_1 + 4) = puVar1;
    return;
}
