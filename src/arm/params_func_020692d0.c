extern int func_02069318();
extern int * func_02069470();

unsigned int func_020692d0(int param_1,int *param_2,unsigned int param_3,unsigned int param_4)

{
    int *piVar1;

    piVar1 = func_02069470(param_1,((int)0x41424e4bu));
    if (piVar1 == (int *)0x0) {
        *param_2 = 0;
        return 0;
    }
    func_02069318((unsigned short *)(piVar1 + 2));
    *param_2 = (int)(piVar1 + 2);
    return 1;
}
