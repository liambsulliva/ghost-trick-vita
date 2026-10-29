typedef int code();
extern int func_02056cac();

unsigned int * func_020640dc(unsigned int *param_1)

{
    unsigned int uVar1;

    uVar1 = 0;
    *param_1 = ((unsigned int)0x020afd14u);
    do {
        (**(code **)(*(int *)param_1[uVar1 + 0x32] + 0x20))((int *)param_1[uVar1 + 0x32],0);
        uVar1 = uVar1 + 1;
    } while (uVar1 < 4);
    func_02056cac(param_1);
    return param_1;
}
