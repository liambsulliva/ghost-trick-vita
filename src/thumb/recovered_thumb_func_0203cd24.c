typedef int code();
extern int func_0205765c();
extern int func_02057994();

void func_0203cd24(int param_1)

{
    int *piVar1;

    piVar1 = (int *)func_02057994(0x24,0x70,0,0,1);
    *(int **)(param_1 + 0x24) = piVar1;
    if (piVar1 != (int *)0x0) {
        (**(code **)(*piVar1 + 0xb8))(piVar1,0x100000,0xc0000);
        func_0205765c(*(unsigned int *)(param_1 + 0x24),0,0,0x100000,0xc0000);
    }
    return;
}
