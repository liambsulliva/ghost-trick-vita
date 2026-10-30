typedef struct {
    int pad[3];
    int offset;
    int addr;
} State02070ba4;

extern int data_020b1748;
extern State02070ba4 data_020d0e48;
void func_02072bc8(int, int, int, unsigned int, int, int, int);
void func_02072ed8(int, int, unsigned int);

void func_02070ba4(int param_1, int param_2, unsigned int param_3)
{
    int dest;

    dest = data_020d0e48.addr + param_2 - data_020d0e48.offset;
    if (data_020b1748 != -1) {
        func_02072bc8(data_020b1748, param_1, dest, param_3, 0, 0, 1);
        return;
    }
    func_02072ed8(param_1, dest, param_3);
}
