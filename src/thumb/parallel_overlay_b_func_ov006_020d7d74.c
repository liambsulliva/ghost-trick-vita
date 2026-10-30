extern int func_ov006_020d813c();
extern void func_ov006_020d7108();
extern int func_0200847c();
extern void func_0202a0c0();
extern void func_ov006_020d7be8();

void func_ov006_020d7d74(unsigned char *param_1, int param_2)
{
    param_1[0x26] = 4;
    *(unsigned short *)(param_1 + 0x3de) = 0;
    *(unsigned int *)(param_1 + 0x434) = func_ov006_020d813c(0xae, 0, 2);
    func_ov006_020d7108(param_1, param_2, 0);
    func_0202a0c0(func_0200847c(), 2);
    if (*(signed char *)(param_1 + 0x2b) != 0) {
        param_1[0x2b] = 0;
        func_ov006_020d7be8(param_1, param_2);
    }
}
