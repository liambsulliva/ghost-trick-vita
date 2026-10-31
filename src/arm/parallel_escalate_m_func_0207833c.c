extern unsigned int func_02078278(void *data, int length);
extern int func_02078a74(int a, int b, int c, int d);

unsigned int func_0207833c(unsigned int param_1, unsigned int param_2)
{
    unsigned int command[2];

    command[0] = 0x03006000;
    func_02078278(command, 1);
    while (func_02078a74(0, 2, 0, 1) != 1) {
    }
    command[0] = 0x02006200 | (param_1 & 0xff);
    command[1] = 0x01010000 | (param_2 & 0xffff);
    func_02078278(command, 2);
    return 0;
}
