extern void *func_0202f624(int size);
extern int func_02038e04();
extern int func_0203971c();
extern void func_02039264(void *item);
extern int func_02039f34();

int *func_02039ee4(int param_1, unsigned char param_2, unsigned char param_3, int param_4)
{
    int *piVar1;

    piVar1 = (int *)func_0202f624(0xe8);
    if (piVar1 != 0) {
        piVar1 = (int *)func_02038e04(piVar1, param_2, param_3, *(unsigned char *)(param_1 + 0x11));
    }
    if (piVar1 != 0) {
        if (param_4 != 0) {
            func_0203971c(piVar1, 1);
        }
        else {
            func_0203971c(piVar1, 0);
        }
        func_02039264(piVar1);
        func_02039f34(piVar1, *(char *)(param_1 + 0x10) == 0);
    }
    return piVar1;
}
