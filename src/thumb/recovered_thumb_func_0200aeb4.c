extern int func_02007ab4();
extern int func_0200af10();
extern int func_0200cb68();

unsigned int * func_0200aeb4(unsigned int *param_1)

{
    *param_1 = ((unsigned int)0x020a2cd4u);
    if (param_1[4] != 0) {
        func_0200cb68(param_1[4],param_1[1]);
    }
    if (param_1[5] != 0) {
        func_0200cb68(param_1[5],param_1[2]);
    }
    func_02007ab4(param_1);
    func_0200af10(param_1);
    return param_1;
}
