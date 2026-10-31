extern char data_020ad344;
extern char data_020cf724;
extern char data_020cf470;
extern int func_0202f524();
extern void func_0200f70c(void *object);
extern int func_0205693c();
extern int func_02054074();
extern void func_020153d8();
extern char *func_02009900();
extern void func_0202f7e8();

int *func_020540d4(int *param_1)
{
    char *puVar2;

    func_0202f524(param_1);
    param_1[10] = 0;
    *param_1 = (int)&data_020ad344;
    *((char *)param_1 + 0x3e) = 0;
    *((char *)param_1 + 0x3f) = 0;
    *((char *)param_1 + 0x40) = 0;
    *((char *)param_1 + 0x41) = 0;
    *(short *)((char *)param_1 + 0x42) = 0;
    *(short *)((char *)param_1 + 0x44) = 0;
    *((char *)param_1 + 0x4a) = 0;
    *(short *)((char *)param_1 + 0x4c) = 0;
    *(short *)((char *)param_1 + 0x34) = 0;
    *(short *)((char *)param_1 + 0x36) = 0;
    *(short *)((char *)param_1 + 0x38) = 0;
    *(short *)((char *)param_1 + 0x3a) = 0;
    *(short *)((char *)param_1 + 0x3c) = 0;
    param_1[20] = 0;
    func_0200f70c(&param_1[0x16]);
    param_1[20] = func_0205693c(&data_020cf724);
    (*(int (**)())(*(int *)param_1[20] + 0x2c))(param_1[20]);
    (*(int (**)())(*(int *)param_1[20] + 0x94))(param_1[20], 0x32);
    func_020153d8(param_1[20], param_1);
    puVar2 = func_02009900();
    func_0202f7e8(puVar2, param_1[20], 7, 0);
    param_1[21] = func_02054074(&data_020cf470);
    func_020153d8(param_1[21], param_1);
    puVar2 = func_02009900();
    func_0202f7e8(puVar2, param_1[21], 7, 0);
    *((char *)param_1 + 0x40) = 0;
    *((char *)param_1 + 0x41) = 0;
    *((char *)param_1 + 0x10) = 0;
    return param_1;
}
