extern int func_02031288();
extern int func_020312a4();

void func_0201bee8(unsigned int *param_1,unsigned int param_2)

{
    unsigned int uVar1;
    unsigned int uVar2;

    if (*param_1 < param_2) {
        uVar1 = func_02031288(param_2 << 2);
        for (uVar2 = 0; uVar2 < *param_1; uVar2 = uVar2 + 1) {
            *(unsigned int *)(uVar1 + uVar2 * 4) = *(unsigned int *)(param_1[2] + uVar2 * 4);
        }
        if (param_1[2] != 0) {
            func_020312a4();
            param_1[2] = 0;
        }
        param_1[2] = uVar1;
        param_1[1] = param_2;
    }
    return;
}
