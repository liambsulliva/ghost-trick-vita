extern void func_0206ed14(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int *);
extern void func_0206f760(unsigned int *);

void func_0206f644(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4, unsigned int param_5, unsigned int param_6, unsigned int param_7, unsigned int param_8, unsigned int *param_9)
{
    unsigned int aiStack_48[16];

    if (param_9 == 0) {
        param_9 = aiStack_48;
    }
    func_0206ed14(param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_9);
    if (param_8 == 0) {
        return;
    }
    *(unsigned int *)0x04000440 = 0;
    func_0206f760(param_9);
}
