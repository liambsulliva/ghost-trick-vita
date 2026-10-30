extern int func_0202f64c();

void func_ov005_020d4500(int param_1)
{
    signed char i;

    i = 0;
    do {
        func_0202f64c(param_1 + 0x20 + i * 4);
        i = i + 1;
    } while (i < 5);
    func_0202f64c(param_1 + 0x3c);
    func_0202f64c(param_1 + 0x40);
    func_0202f64c(param_1 + 0x34);
    func_0202f64c(param_1 + 0x38);
    *(unsigned char *)(param_1 + 0x51) = *(unsigned char *)(param_1 + 0x51) & ~1;
}
