extern int func_0202f64c();
extern int func_0203a71c();

void func_0203a4a4(int *param_1,unsigned int param_2)

{
    if (*param_1 != 0) {
        if (param_2 <= 1) {
            func_0203a71c(*param_1,param_2);
            *param_1 = 0;
            return;
        }
        func_0202f64c((unsigned char *)param_1);
    }
    return;
}
