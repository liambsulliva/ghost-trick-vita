typedef int code();

void func_02014dec(int *param_1)
{
    switch (param_1[10]) {
    case 0:
        (*(code **)(*param_1 + 0x30))(param_1);
        return;
    case 1:
        (*(code **)(*param_1 + 0x34))(param_1);
        return;
    case 2:
        (*(code **)(*param_1 + 0x38))(param_1);
        break;
    case 3:
        break;
    }
}
