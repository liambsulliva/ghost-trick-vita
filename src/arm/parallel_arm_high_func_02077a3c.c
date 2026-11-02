extern int func_02077cc4();

int func_02077a3c(unsigned short *param_1)
{
    unsigned char *pbVar1;
    unsigned short uVar2;
    unsigned short uVar3;
    unsigned short uVar4;
    unsigned short uVar5;
    unsigned char bVar6;
    unsigned char bVar7;
    unsigned char bVar8;
    unsigned char bVar9;

    pbVar1 = (unsigned char *)0x02fffc80;
    uVar2 = *(unsigned short *)(pbVar1 + 0x58);
    uVar3 = *(unsigned short *)(pbVar1 + 0x5a);
    uVar4 = *(unsigned short *)(pbVar1 + 0x5e);
    uVar5 = *(unsigned short *)(pbVar1 + 0x60);
    bVar6 = pbVar1[0x5c];
    bVar7 = pbVar1[0x5d];
    bVar8 = pbVar1[0x62];
    bVar9 = pbVar1[0x63];
    if ((uVar2 == 0 && uVar4 == 0 && uVar3 == 0 && uVar5 == 0) ||
        func_02077cc4(param_1, uVar2, uVar3, bVar6, bVar7, uVar4, uVar5, bVar8, bVar9) != 0) {
        param_1[0] = 0;
        param_1[1] = 0;
        param_1[2] = 0;
        param_1[3] = 0;
        return 1;
    }
    return 1;
}
