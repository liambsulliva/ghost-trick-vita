extern int func_02001470();

unsigned int func_02006fac(char *param_1,int param_2)

{
    unsigned int uVar1;

    uVar1 = func_02001470(param_1,param_2);
    if (uVar1 + 3 <= 1) {
        uVar1 = 0xfffffffd;
    }
    return uVar1;
}
