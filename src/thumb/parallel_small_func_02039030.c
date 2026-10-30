extern void func_0200b518(unsigned char *object, int index, int first, int second);
extern void func_0200b598(unsigned int *object, unsigned int value, unsigned int count);
extern void func_020280a8(void *object, int value);

void func_02039030(unsigned char *param_1)
{
    unsigned char kind;

    kind = param_1[0x10];
    if (kind != 0x35 && kind != 0x37 && kind != 0x32) {
        param_1[0x10] = 0;
    }
    func_0200b518(param_1 + 0x28, 0, 0x80000000, 0x80000000);
    func_0200b598((unsigned int *)(param_1 + 0x28), *(unsigned int *)(param_1 + 0xbc), 0xf);
    func_020280a8(param_1 + 0x28, 5);
}
