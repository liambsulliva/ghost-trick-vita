extern unsigned int *func_0200c9d8();
extern char data_020a2da0;

unsigned int *func_0200cde8(unsigned int *param_1)
{
    func_0200c9d8(param_1, 0x10, 0x10, 0x200, 0x20);
    *param_1 = (unsigned int)&data_020a2da0;
    param_1[0x11] = 0x20;
    return param_1;
}
