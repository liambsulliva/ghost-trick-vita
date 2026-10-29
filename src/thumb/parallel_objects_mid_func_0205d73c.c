extern int func_0202f64c();

void func_0205d73c(int param_1)

{
    unsigned char uVar1;

    uVar1 = 0;
    do {
        func_0202f64c((unsigned char *)(param_1 + 0x84 + uVar1 * 4));
        uVar1 = uVar1 + 1;
    } while (uVar1 < 2);
    *(unsigned char *)(param_1 + 0x60) = 0;
    return;
}
