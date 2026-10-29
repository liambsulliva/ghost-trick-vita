extern int func_0202a4e8();
extern int func_0202a60c();
extern int func_0202a678();

void func_0202a5dc(int param_1,int param_2)

{
    unsigned int uVar1;

    if (*(char *)(param_1 + 0x24) != '\x02') {
        uVar1 = func_0202a4e8(param_1,*(unsigned short *)(param_1 + param_2 * 0x10 + 0x3a));
        func_0202a60c(param_1,param_2);
        func_0202a678(param_1,uVar1);
    }
    return;
}
