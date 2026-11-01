extern int func_0203c6a8();
void func_0203d404(int param_1)
{
    int iVar1;
    unsigned char bVar1;
    iVar1 = func_0203c6a8(param_1, 1, 0, 0);
    if (iVar1 == 0) {
        bVar1 = *(unsigned char *)(param_1 + 4);
        if ((unsigned char)(bVar1 + 0xff) <= 2) {
            if (bVar1 == 3) {
                *(unsigned char *)(param_1 + 0x50) = 1;
                *(unsigned char *)(param_1 + 0x55) = 1;
            } else {
                *(unsigned char *)(param_1 + 0x55) = 0;
            }
            *(unsigned char *)(param_1 + 4) = 12;
            *(unsigned short *)(param_1 + 0x4a) = 0;
        }
    }
}
