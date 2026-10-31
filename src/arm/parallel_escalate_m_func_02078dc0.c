extern unsigned int data_020b17a8;
extern void func_02075d14(unsigned int cycles);
extern int func_02078cb0(void);
extern int func_0207855c(int a, int b);
extern int func_02078c74(int a);

void func_02078dc0(void)
{
    unsigned int saved;

    func_02075d14(0x360000);
    saved = data_020b17a8;
    data_020b17a8 = 14;

    if (func_02078cb0() != 1) {
        while (func_0207855c(2, 0) != 0) {
            func_02075d14(0x51d23);
        }
        while (func_02078c74(1) == 0) {
            func_02075d14(5);
        }
    }

    data_020b17a8 = saved;
}
