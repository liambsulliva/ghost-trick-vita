struct Bits {
    unsigned char low : 3;
    unsigned char high : 5;
};

extern void func_020153d8(int object, int value);
extern void func_020083b0(int *dst, int *src);
extern int data_020a46c8;

void func_02044714(char *param_1, int param_2, int param_3, int param_4)
{
    func_020153d8((int)param_1, param_2);
    func_020083b0((int *)(param_1 + 0x28), &data_020a46c8);
    ((struct Bits *)(param_1 + 0xe0))->high = (unsigned char)param_3;
    *(int *)(param_1 + 0xd0) = param_4;
}
