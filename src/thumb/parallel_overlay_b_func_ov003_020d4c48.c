typedef int code();
extern void func_0200ee34(char *param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);

void func_ov003_020d4c48(char *param_1)
{
    int i;
    char *slot;
    int *obj;
    int *other;
    int *next;
    int result;

    for (i = 0; i < *(int *)(param_1 + 0x180); i = i + 1) {
        slot = param_1 + i * 4;
        obj = *(int **)(slot + 0x44);
        if (obj != (int *)0) {
            other = *(int **)(slot + 0x8c);
            if (other != (int *)0) {
                result = (**(code **)(*obj + 0x44))(obj);
                other = *(int **)(slot + 0x8c);
                (**(code **)(*other + 0x30))(other, result);
                obj = *(int **)(slot + 0x44);
                result = (**(code **)(*obj + 0x68))(obj);
                other = *(int **)(slot + 0x8c);
                (**(code **)(*other + 0x54))(other, result);
                if (i == *(int *)(param_1 + 0x178)) {
                    func_0200ee34(*(char **)(slot + 0x8c), 0x1f, 0, 0);
                } else {
                    func_0200ee34(*(char **)(slot + 0x8c), 0x1c, 0xa, 0);
                }
            }
            next = *(int **)(slot + 0xd4);
            if (next != (int *)0) {
                obj = *(int **)(slot + 0x44);
                result = (**(code **)(*obj + 0x44))(obj);
                next = *(int **)(slot + 0xd4);
                (**(code **)(*next + 0x30))(next, result);
            }
        }
    }
}
