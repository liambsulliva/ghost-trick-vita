extern unsigned char * func_0200847c();
extern int func_0202ab38();

void func_020619d0(int param_1)
{
    unsigned char *puVar1;

    puVar1 = func_0200847c();
    func_0202ab38((int)puVar1,*(unsigned short *)(param_1 + 0x2c) & 0xff,(unsigned int)*(unsigned short *)(param_1 + 0x30),
                  (int)*(short *)(param_1 + 0x32),*(unsigned short *)(param_1 + 0x2e));
    *(unsigned int *)(param_1 + 0x6c) = 4;
    *(unsigned char *)(param_1 + 0x58) = *(unsigned char *)(param_1 + 0x58) & ~0x10;
    *(unsigned char *)(param_1 + 0x58) = *(unsigned char *)(param_1 + 0x58) & ~0x20;
    return;
}
