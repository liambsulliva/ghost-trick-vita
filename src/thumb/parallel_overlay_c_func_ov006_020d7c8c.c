extern int func_ov006_020d7ce0(int);
extern void func_ov006_020d7fe8(int);
extern void func_ov006_020d7ff4(int);

void func_ov006_020d7c8c(int param_0, int param_1, int param_2, int param_3, signed char param_4)
{
    int iVar1;

    if ((param_1 != 0) && (iVar1 = func_ov006_020d7ce0(param_1), iVar1 == 0)) {
        if (param_3 <= 0) {
            func_ov006_020d7ff4(param_1);
        }
        else {
            func_ov006_020d7fe8(param_1);
        }
    }
    if ((param_2 != 0) && (iVar1 = func_ov006_020d7ce0(param_2), iVar1 == 0)) {
        if (param_3 >= param_4 - 1) {
            func_ov006_020d7ff4(param_2);
        }
        else {
            func_ov006_020d7fe8(param_2);
        }
    }
}
