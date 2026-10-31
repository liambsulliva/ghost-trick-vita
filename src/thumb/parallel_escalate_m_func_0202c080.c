struct Flags {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
};

extern void func_0202c124(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
extern void func_0202c160(unsigned int, int);
extern void func_0202c17c(unsigned int, unsigned int, unsigned int, int, int, unsigned int);

void func_0202c080(int param_1)
{
    int level;

    if (*(int *)(param_1 + 0x48) != 0) {
        if (((struct Flags *)(param_1 + 0x78))->bit0) {
            func_0202c124(*(unsigned char *)(param_1 + 0x62), 1, *(unsigned char *)(param_1 + 0x60), *(unsigned char *)(param_1 + 0x61), 0, 0, 0, *(unsigned int *)(param_1 + 0x50));
        } else {
            func_0202c124(*(unsigned char *)(param_1 + 0x62), 1, *(unsigned char *)(param_1 + 0x60), *(unsigned char *)(param_1 + 0x61), 0, 0, 1, *(unsigned int *)(param_1 + 0x50));
        }
        if (((struct Flags *)(param_1 + 0x78))->bit2 == 0 || ((struct Flags *)(param_1 + 0x78))->bit1 == 0) {
            func_0202c160(*(unsigned int *)(param_1 + 0x54), *(unsigned char *)(param_1 + 0x62));
        } else {
            func_0202c160(*(unsigned int *)(param_1 + 0x58), *(unsigned char *)(param_1 + 0x62));
        }
    } else {
        func_0202c124(0, 0, 0, 0, 0, 0, 0, 0);
    }
    level = *(signed char *)(param_1 + 0x44);
    if (level > 0x1f) {
        level = 0x1f;
    }
    func_0202c17c(0, 0, 3, *(signed char *)(param_1 + 0x45), level, *(unsigned int *)(param_1 + 0x64));
}
