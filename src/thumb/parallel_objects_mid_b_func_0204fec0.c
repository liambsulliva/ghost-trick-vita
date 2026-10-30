typedef int (*slot_fn)(int *, int, int);

extern int func_0204faf4(int *);

int func_0204fec0(int param_1, int param_2, int param_3)
{
    int i;
    int *puVar1;
    int *piVar3;
    int iVar5;
    int result;

    for (i = 0; i < 12; i++) {
        iVar5 = param_1 + i * 4;
        puVar1 = *(int **)(iVar5 + 0x48);
        if (puVar1 != 0) {
            if (func_0204faf4(puVar1) == 1) {
                piVar3 = *(int **)(iVar5 + 0x48);
                result = (*(slot_fn *)(*piVar3 + 0xc0))(piVar3, param_2, param_3);
                if (result != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}
