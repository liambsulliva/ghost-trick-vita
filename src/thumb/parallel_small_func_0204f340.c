extern void func_0204ef08(unsigned int object);
extern void func_0204f22c(int *object, int a, int b, int c, int d, int e, int f, int g);
extern void func_0204f290(int object, int a, int b);
extern void func_0200ee2c(unsigned char *object, int value);

void func_0204f340(int *param_1)
{
    int value;

    func_0204ef08((unsigned int)param_1);
    func_0204f22c(param_1, 0, 0xa8, 200, 0x228, 0x22d, 0x228, 0x22d);
    value = 2;
    func_0204f290((int)param_1, 2, value - 3);
    value = value + 0xfe;
    func_0200ee2c((unsigned char *)param_1, value);
}
