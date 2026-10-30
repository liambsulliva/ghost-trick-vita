extern void func_0207652c(int, int, int, int);
extern void func_02076660(int, int);
extern void func_02076b60(int);

void func_ov004_020d5b38(int param_1)
{
    if (((((unsigned int)*(unsigned char *)(param_1 + 0xaaa)) << 27) >> 30) == 1) {
        func_0207652c(1, 0, 1, 0);
        func_02076660(1, 0);
    } else {
        func_0207652c(3, 0, 1, 0);
        func_02076660(3, 0);
    }
    func_02076b60(1);
}
