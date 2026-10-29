extern int func_020312a4();
extern int func_0207ddb8();

void func_0207dea4(int param_1,int param_2,int param_3,unsigned char *param_4)

{
    if (param_1 == 0) {
        return;
    }
    if (param_4 != (unsigned char *)0x0) {
        func_0207ddb8(param_1,*(int *)(param_1 + -4),param_2,param_4);
    }
    func_020312a4(param_1 - param_3);
    return;
}
