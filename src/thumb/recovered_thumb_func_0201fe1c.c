extern int func_0201fd84();
extern int func_0201fde4();
extern int func_0201fe00();

unsigned int * func_0201fe1c(unsigned int *param_1)

{
    func_0201fde4();
    *param_1 = ((unsigned int)0x020a48e8u);
    func_0201fe00(param_1 + 4,0x60);
    func_0201fd84(param_1,param_1 + 4,0x18);
    return param_1;
}
