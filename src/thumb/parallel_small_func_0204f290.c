void func_0204f290(signed char *param_1,signed char param_2,signed char param_3)

{
    signed char cVar1;

    cVar1 = param_1[0xc6];
    param_1[0xc5] = cVar1;
    param_1[0xc6] = param_2;
    param_1[0xc7] = param_3;
    param_1[0xe0] = 0;
    if (param_1[0xc6] == 4) {
        param_1[0xc8] = 0;
    }
    if (param_1[0xc5] == 4) {
        if (param_1[0xc6] == 2) {
            param_1[0xc8] = 0;
            return;
        }
        if (param_1[0xc6] == 3) {
            param_1[0xc8] = 8;
        }
    }
    return;
}
