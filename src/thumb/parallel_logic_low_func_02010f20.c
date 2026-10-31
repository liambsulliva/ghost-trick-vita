extern int func_02010f74();
extern int func_0204fbf0();

void func_02010f20(unsigned char *param_1, unsigned int param_2)
{
    unsigned int uVar1;
    int *piVar2;

    if (*(int *)(param_1 + 0xfb8) != 0) {
        uVar1 = 0;
        piVar2 = (int *)(param_1 + 0xfb8);
        do {
            if (((uVar1 != 5) || ((param_2 != 3 && (param_2 != 5)))) &&
                  ((1 < (uVar1 + 0xfa & 0xff) || (param_2 != 5)))) {
                func_0204fbf0(*piVar2,uVar1,param_2);
            }
            uVar1 = uVar1 + 1 & 0xff;
        } while (uVar1 < 0xc);
        if (param_2 == 2) {
            func_02010f74(*(int *)(param_1 + 0xfb8));
        }
    }
    return;
}
