typedef int code();
extern int func_0203bfb4();

void func_0203cf74(int param_1, int param_2, int param_3, unsigned int param_4)
{
    unsigned int y;

    y = 0xffff0000u;
    if (param_2 != 0) {
        (**(code **)(**(int **)(param_1 + 0x38) + 0x34))(*(int **)(param_1 + 0x38), 0x2c000, y, 0);
        (**(code **)(**(int **)(param_1 + 0x3c) + 0x34))(*(int **)(param_1 + 0x3c), 0xfffd4000u, y, 0);
    } else {
        (**(code **)(**(int **)(param_1 + 0x38) + 0x34))(*(int **)(param_1 + 0x38), 0xfffd4000u, y, 0);
        (**(code **)(**(int **)(param_1 + 0x3c) + 0x34))(*(int **)(param_1 + 0x3c), 0x2c000, y, 0);
    }
    if (param_3 != 0) {
        *(unsigned char *)(param_1 + 5) = 1;
        func_0203bfb4(*(unsigned char **)(param_1 + 0x38), 0);
        func_0203bfb4(*(unsigned char **)(param_1 + 0x3c), 1);
        return;
    }
    *(unsigned char *)(param_1 + 5) = 0;
    func_0203bfb4(*(unsigned char **)(param_1 + 0x38), 1);
    func_0203bfb4(*(unsigned char **)(param_1 + 0x3c), 0);
}
