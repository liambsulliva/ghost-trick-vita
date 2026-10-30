void func_0207250c(int param_1,unsigned int param_2)
{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned int uVar3;

    uVar2 = 0;
    do
    {
        uVar1 = uVar2;
        uVar3 = 0;
        do
        {
            if ((uVar1 & 1) != 0)
            {
                uVar1 = param_2 ^ (uVar1 >> 1);
            }
            else
            {
                uVar1 = uVar1 >> 1;
            }
            uVar3 = uVar3 + 1;
        } while (uVar3 < 8);
        *(unsigned int *)(param_1 + uVar2 * 4) = uVar1;
        uVar2 = uVar2 + 1;
    } while (uVar2 < 0x100);
    return;
}
