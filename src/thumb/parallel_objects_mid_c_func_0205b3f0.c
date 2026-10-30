extern int func_0205b0b0();
extern int func_0205ad2c();

int func_0205b3f0(int param_1,unsigned int param_2)
{
    unsigned int uVar3;
    unsigned int uVar4;
    int iVar2;

    for (uVar3 = 0; uVar3 < 0x20; uVar3 = uVar3 + 1) {
        uVar4 = uVar3 * 4;
        iVar2 = *(int *)(param_1 + uVar4 + 0x44);
        if (iVar2 != 0 && func_0205b0b0(iVar2) == param_2) {
            return func_0205ad2c(*(int *)(param_1 + uVar4 + 0x44));
        }
    }
    return 0;
}
