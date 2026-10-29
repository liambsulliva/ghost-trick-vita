extern int func_02038790();
extern int func_0203a028();

void func_02039eb8(unsigned char *param_1,unsigned int param_2,int param_3)

{
    unsigned int *puVar1;

    puVar1 = (unsigned int *)func_02038790(param_1);
    if (puVar1 != (unsigned int *)0x0) {
        for (; puVar1 != (unsigned int *)0x0; puVar1 = (unsigned int *)*puVar1) {
            if ((int *)puVar1[2] != (int *)0x0) {
                func_0203a028(param_1,(int *)puVar1[2],param_2,param_3);
            }
        }
    }
    return;
}
