extern unsigned int data_020d0f0c;
extern void func_02074404();
extern void func_020744bc();
extern void func_0207403c();

void func_02073fe4(unsigned int *param_1,unsigned int param_2)
{
    if (((unsigned int *)&data_020d0f0c)[5] != 0) {
        func_02074404(param_1,(unsigned int)func_0207403c);
        param_1[0] = param_1[0] | 128;
        param_1[1] = param_2;
        param_1[25] = 1;
        func_020744bc(param_1);
        return;
    }
    func_0207403c(param_2);
}
