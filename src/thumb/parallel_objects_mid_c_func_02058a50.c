extern int func_020083b0();
extern int func_02008350();

void func_02058a50(int param_1, int *param_2)
{
    int flag;

    if (*(int *)(param_1 + 4) == 0) {
        func_020083b0(param_1 + 8);
        return;
    }
    flag = func_02008350(*(int *)(param_1 + 4));
    if (flag != 0) {
        *(int *)(param_1 + 8) = -*param_2;
    } else {
        *(int *)(param_1 + 8) = *param_2;
    }
    *(int *)(param_1 + 0xc) = param_2[1];
    *(int *)(param_1 + 0x10) = 0;
}
