struct HeapState {
    unsigned int lock;
    unsigned int f4;
    unsigned int f8;
    unsigned int fc;
    unsigned int f10;
};

extern struct HeapState data_020cffa8;
extern unsigned int data_020cffac[];
extern unsigned int func_02068a98(unsigned int a, unsigned int b);
extern void func_02068a8c(unsigned int *lock);
extern void func_02068adc(unsigned int *lock, unsigned int *object, int a, unsigned int b);
extern void func_02068c84(unsigned int *lock, unsigned int *object);

void func_02069274(void)
{
    data_020cffa8.f4 = func_02068a98(data_020cffa8.fc, data_020cffa8.f10 >> 4);
    func_02068a8c(data_020cffac - 1);
    func_02068adc(data_020cffac - 1, data_020cffac, 0, data_020cffa8.f8);
    func_02068c84(data_020cffac - 1, data_020cffac);
}
