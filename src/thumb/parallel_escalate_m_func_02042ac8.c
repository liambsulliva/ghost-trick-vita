extern void *func_0200eb78();
extern int func_02022c8c();
extern void *func_02007bd8();
extern int func_02007b70();
typedef int code();

void func_02042ac8(unsigned char *param_1)
{
    int iVar4;
    int *piVar3;
    unsigned char *puVar2;
    unsigned short *puVar1;

    if (*(int *)(param_1 + 8) != -1) {
        *(int *)(param_1 + 8) = -1;
        param_1[0x39] = 0;
    }
    iVar4 = *(int *)(param_1 + 0x14);
    if (iVar4 != -1) {
        puVar1 = func_0200eb78();
        func_02022c8c(puVar1, iVar4);
        *(int *)(param_1 + 0x14) = -1;
    }
    if (*(int *)(param_1 + 0x50) != 0) {
        puVar2 = func_02007bd8();
        iVar4 = func_02007b70(puVar2);
        if (iVar4 != 0) {
            puVar2 = func_02007bd8();
            piVar3 = (int *)func_02007b70(puVar2);
            (**(code **)(*piVar3 + 0x10))(piVar3, *(int *)(param_1 + 0x50));
            *(int *)(param_1 + 0x50) = 0;
        }
    }
}
