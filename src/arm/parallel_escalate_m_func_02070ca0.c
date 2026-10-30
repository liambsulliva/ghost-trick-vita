typedef struct {
    int pad;
    int base;
} State02070ca0;

extern int data_020b1748;
extern State02070ca0 data_020d0e48;
void func_02072bc8(int, int, int, unsigned int, int, int, int);
void func_02072ed8(int, int, unsigned int);

void func_02070ca0(int param_1, int param_2, unsigned int param_3)
{
    int base;

    base = data_020d0e48.base;
    if (data_020b1748 != -1) {
        func_02072bc8(data_020b1748, param_1, base + param_2, param_3, 0, 0, 1);
        return;
    }
    func_02072ed8(param_1, base + param_2, param_3);
}
