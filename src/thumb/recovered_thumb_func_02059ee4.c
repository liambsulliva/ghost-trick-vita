extern int func_0200f70c();
extern int func_02062c30();

unsigned int * func_02059ee4(unsigned int *param_1)

{
    func_02062c30();
    *param_1 = ((unsigned int)0x020ae828u);
    func_0200f70c(param_1 + 0x3f);
    func_0200f70c(param_1 + 0x42);
    param_1[4] = 0;
    return param_1;
}
