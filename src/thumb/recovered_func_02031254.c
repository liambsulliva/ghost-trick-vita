typedef int code();
extern int func_02007b70();
extern int func_02007bd8();

void func_02031254(unsigned int param_1)

{
    int *piVar1;

    func_02007bd8();
    piVar1 = (int *)func_02007b70();
    (**(code **)(*piVar1 + 8))(piVar1,param_1,((unsigned int)0x02086844u));
    return;
}
