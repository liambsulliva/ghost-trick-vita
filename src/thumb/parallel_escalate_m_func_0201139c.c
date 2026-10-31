extern int func_020112a8();
extern unsigned char *func_02011264();
extern unsigned char *func_0201123c();
extern int func_02022f2c();
extern unsigned char *func_02008444();
extern void func_020113f8();
extern char data_020ce1e8;
extern char data_020ce444;

void func_0201139c(unsigned char *param_1)
{
    unsigned int uVar4;
    int iVar1;
    unsigned char *puVar2;
    uVar4 = *(unsigned int *)(param_1 + 0xc);
    if ((((unsigned int)uVar4) << 7) >> 31) {
        if ((((unsigned int)uVar4) << 9) >> 31) {
            *(unsigned int *)(param_1 + 0xc) = 0xfeffffffu & uVar4;
            iVar1 = func_020112a8(param_1);
            if (iVar1 != 0) {
                puVar2 = func_02011264();
                puVar2 = func_0201123c(puVar2, 1);
                func_02022f2c(puVar2, &data_020ce1e8, 0);
                return;
            }
            func_020113f8(func_02008444());
            puVar2 = func_02011264();
            puVar2 = func_0201123c(puVar2, 1);
            func_02022f2c(puVar2, &data_020ce444, 0);
        }
    }
}
