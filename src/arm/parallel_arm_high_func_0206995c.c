extern int func_020699b4();
extern void func_02072ef0(void *dst, void *src, unsigned int size);
extern unsigned int data_020d0220[];
extern unsigned int data_04000400[];

void func_0206995c(unsigned int param_1, unsigned int param_2, int param_3)
{
    unsigned int *puVar1;
    int *piVar2;

    if (data_020d0220[1] != 0) {
        func_020699b4();
    }
    puVar1 = data_020d0220;
    piVar2 = (int *)*puVar1;
    if (piVar2 != 0) {
        param_3 = *piVar2;
        if (param_3 != 0) {
            func_02072ef0(piVar2 + 1, data_04000400, param_3 << 2);
            *(unsigned int *)*puVar1 = 0;
        }
    }
}
