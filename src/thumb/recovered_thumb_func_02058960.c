extern int func_02007ab8();
extern int func_02058bc8();

unsigned int * func_02058960(unsigned int *param_1)

{
    func_02058bc8();
    *param_1 = ((unsigned int)0x020ae794u);
    func_02007ab8(param_1 + 0x45);
    param_1[0x50] = 0;
    param_1[0x51] = 0;
    param_1[0x52] = 0;
    return param_1;
}
