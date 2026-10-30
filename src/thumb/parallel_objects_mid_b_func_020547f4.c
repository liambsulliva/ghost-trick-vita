extern char data_020ad30c;
extern char data_020cf724;
extern int func_0202f524();
extern int func_0205693c();
extern void func_020153d8();
extern char *func_02009900();
extern void func_0202f7e8();

int *func_020547f4(int *param_1)
{
    char *puVar2;

    func_0202f524(param_1);
    *param_1 = (int)&data_020ad30c;
    param_1[10] = 0;
    param_1[11] = 0;
    param_1[12] = 0;
    param_1[10] = func_0205693c(&data_020cf724);
    (*(int (**)())(*(int *)param_1[10] + 0x2c))(param_1[10]);
    (*(int (**)())(*(int *)param_1[10] + 0x94))(param_1[10], 0x33);
    func_020153d8(param_1[10], param_1);
    puVar2 = func_02009900();
    func_0202f7e8(puVar2, param_1[10], 7, 0);
    param_1[11] = func_0205693c(&data_020cf724);
    (*(int (**)())(*(int *)param_1[11] + 0x2c))(param_1[11]);
    (*(int (**)())(*(int *)param_1[11] + 0x94))(param_1[11], 0x33);
    func_020153d8(param_1[11], param_1);
    puVar2 = func_02009900();
    func_0202f7e8(puVar2, param_1[11], 7, 0);
    param_1[12] = func_0205693c(&data_020cf724);
    (*(int (**)())(*(int *)param_1[12] + 0x2c))(param_1[12]);
    (*(int (**)())(*(int *)param_1[12] + 0x94))(param_1[12], 0x33);
    func_020153d8(param_1[12], param_1);
    puVar2 = func_02009900();
    func_0202f7e8(puVar2, param_1[12], 7, 0);
    param_1[13] = 0;
    param_1[14] = 7 - 0x47;
    *((char *)param_1 + 0x10) = 0;
    return param_1;
}
