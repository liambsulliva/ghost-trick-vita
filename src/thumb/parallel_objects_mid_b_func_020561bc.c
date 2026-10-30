extern char data_020cf5f8;
extern int func_02055b4c();
extern int func_020560f4();
extern void func_0203bfb4();
extern void func_020153d8();
extern char *func_02009900();
extern void func_0202f7e8();

void func_020561bc(int *param_1)
{
    char *puVar2;

    param_1[13] = func_02055b4c(&data_020cf5f8);
    if (param_1[13] != 0) {
        func_020560f4(param_1[13], 0x282, 0x287, 0x28c, 0xc2, 0x38);
        func_0203bfb4(param_1[13], 0);
        (*(int (**)())(*(int *)param_1[13] + 0x3c))(param_1[13], 0x2c000);
        func_020153d8(param_1[13], param_1);
        puVar2 = func_02009900();
        func_0202f7e8(puVar2, param_1[13], 7, 0);
    } else {
        param_1[10] = 1;
    }
    if (param_1[11] == 2) {
        param_1[14] = func_02055b4c(&data_020cf5f8);
        *((char *)param_1 + 0x30) = 1;
        if (param_1[14] != 0) {
            func_020560f4(param_1[14], 0x291, 0x296, 0x29b, 0xc2, 0x38);
            func_0203bfb4(param_1[14], 1);
            (*(int (**)())(*(int *)param_1[14] + 0x3c))(param_1[14], 0xffff0000);
            func_020153d8(param_1[14], param_1);
            puVar2 = func_02009900();
            func_0202f7e8(puVar2, param_1[14], 7, 0);
        } else {
            param_1[10] = 1;
        }
    }
}
