extern void func_02017468(int a, int b, int c);

void func_02017418(int *param_1, int param_2)
{
    unsigned int i;

    if (param_1[1] != 0) {
        if (param_2 != 0) {
            for (i = 0; i < (unsigned int)param_1[2]; i++) {
                func_02017468(param_1[1] + (i << 3), 0, 0);
            }
        } else {
            for (i = 0; i < (unsigned int)param_1[2]; i++) {
                func_02017468(param_1[1] + (i << 3), 0x200, 0);
            }
        }
    }
}
