typedef int code();
extern int func_02007b70();
extern unsigned char * func_02007bd8();

typedef struct {
    unsigned int lo : 24;
    unsigned int mid : 7;
    unsigned int top : 1;
} Bits;

void func_02023d1c(int param_1)

{
    unsigned char *puVar1;
    int iVar2;
    int *piVar3;

    if (*(int *)(param_1 + 4) != 0) {
        puVar1 = func_02007bd8();
        iVar2 = func_02007b70(puVar1);
        if (iVar2 != 0) {
            puVar1 = func_02007bd8();
            piVar3 = (int *)func_02007b70(puVar1);
            (**(code **)(*piVar3 + 0x10))(piVar3,*(unsigned int *)(param_1 + 4));
            *(unsigned int *)(param_1 + 4) = 0;
        }
    }
    ((Bits *)(param_1 + 8))->lo = 0;
    ((Bits *)(param_1 + 8))->top = 0;
    *(unsigned int *)(param_1 + 0xc) = 0;
    *(unsigned int *)(param_1 + 0x10) = 0;
    return;
}
