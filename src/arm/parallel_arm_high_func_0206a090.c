extern void func_02074810(void);
extern void func_02074844(unsigned int start, unsigned int size);
extern void func_02074860(unsigned int start, unsigned int size);
extern void func_020748a0(void);

void func_0206a090(unsigned int param_1, unsigned int param_2, unsigned int param_3)
{
    if (param_2 >= param_3) {
        func_02074810();
        return;
    }
    if ((param_1 & 0x1f) != 0) {
        param_1 = param_1 - (param_1 & 0x1f);
        func_02074860(param_1, 0x20);
        func_02074860(param_1 + param_2, 0x20);
        param_2 = param_2 + 0x20;
    }
    func_02074844(param_1, param_2);
    func_020748a0();
}
