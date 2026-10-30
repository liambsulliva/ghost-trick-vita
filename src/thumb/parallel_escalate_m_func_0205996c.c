extern void func_0205963c();
extern void func_0200bc44();
extern int func_02008444();
extern short *func_02046490();
extern void func_02007c40();
extern void func_0205953c();

void func_0205996c(int param_1)
{
    int off;

    off = 0x114;
    func_0205963c(param_1 + 0x28, param_1 + off);
    func_0200bc44(param_1 + 0x28);
    if (*(int *)(param_1 + off + 0x30) != 0) {
        if (((((unsigned int)*(unsigned char *)(param_1 + 0xde)) << 28) >> 31) == 0) {
            func_02007c40(param_1 + off, func_02046490(func_02008444()) + *(int *)(param_1 + off + 0x30));
            func_0205953c(param_1, *(unsigned char *)(param_1 + 0xc3));
        }
    }
}
