void func_02071fe8(unsigned int *param_1)

{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    unsigned int uVar4;
    unsigned int uVar5;

    uVar5 = ((unsigned int)0xc3d2e1f0u);
    uVar4 = ((unsigned int)0x10325476u);
    uVar3 = ((unsigned int)0x98badcfeu);
    uVar2 = ((unsigned int)0xefcdab89u);
    uVar1 = ((unsigned int)0x67452301u);
    param_1[0x16] = 0;
    param_1[0x17] = 0;
    param_1[0x15] = 0;
    *param_1 = uVar1;
    param_1[1] = uVar2;
    param_1[2] = uVar3;
    param_1[3] = uVar4;
    param_1[4] = uVar5;
    return;
}
