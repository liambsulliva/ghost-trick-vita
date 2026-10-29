extern int func_0200b808();
extern int func_0200ba7c();
extern unsigned char func_02010b54();

void func_0202fe8c(int param_1)

{
    unsigned char bVar1;

    if ((*(char *)(param_1 + 0xc) == '\x03') && (bVar1 = func_02010b54(param_1), bVar1)) {
        func_0200ba7c(param_1 + 0x28,(unsigned char *)(param_1 + 0xb0));
        func_0200b808(param_1 + 0x28);
    }
    return;
}
