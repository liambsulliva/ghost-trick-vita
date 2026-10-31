extern unsigned int func_0206ef28();
extern unsigned int func_0200d65c();
extern unsigned int func_0200d668();

void func_0200d590(unsigned int *param_1)
{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    int iVar4;
    unsigned int uVar5;
    unsigned int diff;

    if (param_1[1] == 0) {
        uVar1 = param_1[2];
        param_1[6] = uVar1;
        param_1[5] = uVar1;
        param_1[4] = uVar1;
        goto code_r0x0200d644;
    }
    uVar1 = func_0206ef28(*param_1, 0x3c000);
    uVar2 = func_0200d65c(uVar1);
    diff = uVar1 - uVar2;
    uVar3 = func_0200d668(param_1[2], 0x1000 - param_1[1]);
    iVar4 = func_0200d668(diff, param_1[1]);
    uVar5 = func_0200d668(param_1[2], 0x1000 - iVar4);
    iVar4 = func_0200d668(0x1000 - diff, param_1[1]);
    uVar2 = func_0200d668(param_1[2], 0x1000 - iVar4);
    switch ((int)uVar1 >> 0xc) {
    case 0:
        param_1[4] = param_1[2];
    tail_store:
        param_1[5] = uVar2;
        param_1[6] = uVar3;
        goto code_r0x0200d644;
    case 1:
        param_1[4] = uVar5;
        uVar2 = param_1[2];
        goto tail_store;
    case 2:
        param_1[4] = uVar3;
        param_1[5] = param_1[2];
        goto code_r0x0200d632;
    case 3:
        param_1[4] = uVar3;
        param_1[5] = uVar5;
code_r0x0200d630:
        uVar2 = param_1[2];
code_r0x0200d632:
        param_1[6] = uVar2;
        goto code_r0x0200d644;
    case 4:
        param_1[4] = uVar2;
        param_1[5] = uVar3;
        goto code_r0x0200d630;
    case 5:
        param_1[5] = uVar3;
        param_1[4] = param_1[2];
        param_1[6] = uVar5;
    default:
        goto code_r0x0200d644;
    }
code_r0x0200d644:
    *(unsigned short *)(param_1 + 3) =
        ((int)param_1[4] >> 7) | (((int)param_1[5] >> 7) << 5) | (((int)param_1[6] >> 7) << 10);
}
