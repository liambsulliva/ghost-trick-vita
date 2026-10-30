typedef struct {
    short value;
    unsigned short pad;
    int rest;
} Rec0202ac74;

extern const Rec0202ac74 data_02080e84[];
extern const Rec0202ac74 data_02080e86[];
extern int func_0202acc4(int object, int key);
extern void func_02007184(int a, int b, int c, unsigned int volume, int arg, int slot);

void func_0202ac74(int param_1, int param_2, int param_3, int param_4)
{
    int index;
    unsigned int level;

    if (*(unsigned char *)(param_1 + 0x24) != 1) {
        index = *(int *)(param_1 + param_2 * 0xc + 0x1ec);
        level = (func_0202acc4(param_1, index) * param_3) >> 8;
        if (level > 0x7f) {
            level = 0x7f;
        }
        func_02007184(-1, data_02080e86[index].value, data_02080e84[index].value, level, param_4, param_2);
    }
}
