typedef void code();
void func_0203aa80(int *param_1)
{
    int iVar1;
    int *piVar2;
    iVar1 = *(int *)((char *)param_1 + 0xc8);
    if (param_1[10] > iVar1) {
        param_1[10] = param_1[10] - *(int *)((char *)param_1 + 0xe8);
    } else {
        param_1[10] = iVar1;
        *(unsigned char *)((char *)param_1 + 0x10) = 6;
    }
    piVar2 = (int *)*(int *)((char *)param_1 + 0xf8);
    ((code *)((int *)*piVar2)[14])(piVar2, param_1[10]);
}
