struct Entry {
    unsigned int offset : 24;
    unsigned int flags : 8;
};

extern unsigned int func_02000d64(int a, unsigned int b, void *c, unsigned int d, unsigned int e);
extern void func_02074860(void *start, unsigned int size);
extern void func_0207303c(void *src, void *dst, unsigned int size);
extern void func_020010a0(void *object, int value);

int func_02001020(int param_1, struct Entry *param_2, char *param_3)
{
    unsigned int read;
    unsigned int size;
    unsigned int total;

    read = func_02000d64(param_1, param_2->offset, param_3, 0x3c, 0);
    if (read != 0x3c) {
        return -2;
    }
    size = *(int *)(param_3 + 0x38) * 4;
    total = size + 0x3c;
    read = func_02000d64(param_1, param_2->offset, param_3, total, 0);
    if (read != total) {
        return -2;
    }
    func_02074860(param_3, total);
    func_0207303c(param_3 + 0x3c, param_3 + 0x3c + *(int *)(param_3 + 0x38) * 4, size);
    func_020010a0(param_3 + 0x3c, size);
    func_020010a0(param_3 + 0x3c + *(int *)(param_3 + 0x38) * 8, 0x80);
    return total;
}
