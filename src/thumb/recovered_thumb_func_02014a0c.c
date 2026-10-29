extern int func_0201003c();
extern int func_020152a4();
extern int func_02015990();

void func_02014a0c(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int uVar1;

    if (*(int *)(param_1 + 4) != 0) {
        uVar1 = func_0201003c();
        func_02015990(uVar1,1);
        func_020152a4(*(unsigned int *)(param_1 + 4),param_2,param_4,1,param_3);
    }
    return;
}
