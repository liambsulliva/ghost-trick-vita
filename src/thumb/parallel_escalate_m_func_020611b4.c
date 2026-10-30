extern unsigned int *func_02008444();
extern int func_0201132c();
extern int func_020266dc();
extern int func_0205902c();
extern int func_0206115c();

void func_020611b4(int param_1, unsigned char *param_2, unsigned int param_3, int param_4)
{
    int flags;

    if (func_0201132c((int)func_02008444()) != 0) {
        if (*(char *)(param_1 + 0x7e) == 5) {
            if (param_4 != 0) {
                *(unsigned char **)(param_1 + 0xf8) = param_2;
                *(unsigned int *)(param_1 + 0x100) = func_020266dc((int)param_2, param_3);
            } else {
                *(unsigned char **)(param_1 + 0xf4) = param_2;
                *(unsigned int *)(param_1 + 0xfc) = func_020266dc((int)param_2, param_3);
            }
            flags = *(unsigned char *)(param_1 + 0x7d);
            *(unsigned char *)(param_1 + 0x7d) = flags & ~4;
            *(unsigned char *)(param_1 + 0xb0) = 1;
            func_0205902c(param_2, 0x88);
            return;
        }
        func_0206115c(param_1, (unsigned int)param_2, param_3);
    }
}
