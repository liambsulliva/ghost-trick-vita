extern unsigned char func_02010fa8();
extern int func_0204f290();

void func_0204fb80(unsigned char *param_1,unsigned char param_2,int param_3)

{
    unsigned char bVar1;

    *(unsigned int *)(param_1 + 0x44) = 0;
    *(unsigned char *)(param_1 + 0x2d) = param_2;
    *(unsigned char *)(param_1 + 0x2a) = 1;
    *(signed char *)(param_1 + 0x2c) = -1;
    if (*(signed char *)(param_1 + 0x2d) >= 0) {
        bVar1 = func_02010fa8(param_1,(int)*(signed char *)(param_1 + 0x2d));
        if (bVar1) {
        func_0204f290(*(int *)(param_1 + *(signed char *)(param_1 + 0x2d) * 4 + 0x48),4,1);
        if (param_3 != 4 - 5) {
            *(signed char *)(param_1 + 0x2c) = param_3 + 3;
            return;
        }
    }
        else {
            *(unsigned char *)(param_1 + 0x2b) = 1;
            *(signed char *)(param_1 + 0x2d) = -1;
            return;
        }}
    else {
        *(unsigned char *)(param_1 + 0x2b) = 1;
    }
    return;
}
