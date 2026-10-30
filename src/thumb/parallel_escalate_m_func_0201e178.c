typedef int code();

extern unsigned int func_0201e1c4(int value);
extern int func_02068958(int list, int value);

int func_0201e178(int param_1, int param_2, int param_3)
{
    int result;

    result = 0;
    if (*(int *)(param_1 + 0x30) != 0 && func_0201e1c4(param_2) < *(unsigned int *)(param_1 + 0x34)) {
        result = (*(code **)(**(int **)(param_1 + 0x30) + 8))(*(int **)(param_1 + 0x30), param_2, param_3);
    }
    if (result == 0) {
        if (param_2 < 0) {
            result = func_02068958(param_1 + 0x18, -param_2);
        } else {
            result = func_02068958(param_1 + 8, param_2);
        }
    }
    return result;
}
