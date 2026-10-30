extern unsigned int *func_0200c9d8();
extern char data_020a2d74;

unsigned int *func_0200cfb8(unsigned int *param_1)
{
    func_0200c9d8(param_1, 0x10, 0x10, 0x2000, 0x200);
    *param_1 = (unsigned int)&data_020a2d74;
    param_1[0x11] = 0x200;
    return param_1;
}
