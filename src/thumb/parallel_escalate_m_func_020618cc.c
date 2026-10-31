extern unsigned int * func_02008444();
extern int func_0201135c();
extern int func_0205f074();
extern int func_02060d54();

void func_020618cc(unsigned char *param_1)
{
    int t;

    func_0205f074(*(unsigned char **)(param_1 + 0xdc), 0x76c0);
    *(short *)(param_1 + 0x104) = 0x7fff;
    t = param_1[0x7c];
    param_1[0x7c] = t & ~1;
    *(unsigned short *)(param_1 + 0x84) = 0;
    t = param_1[0x7d];
    param_1[0x7d] = t & ~1;
    func_0201135c(func_02008444());
    func_02060d54(param_1, 3);
}
