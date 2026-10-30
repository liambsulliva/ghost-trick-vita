extern void func_02066564(int param_1);

void func_02066544(int *param_1)
{
    if (param_1[0x32] == 0) {
        (*(void (**)(int *, int *))(*param_1 + 0x20))(param_1, param_1);
        return;
    }
    func_02066564(param_1[0x32]);
}
