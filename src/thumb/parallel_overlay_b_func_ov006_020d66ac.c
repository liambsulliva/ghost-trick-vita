typedef int code(...);

void func_ov006_020d66ac(int *param_1)
{
    int *piVar1;

    piVar1 = *(int **)((int)param_1 + 4);
    (**(code **)(*piVar1 + 0xa0))(piVar1);
}
