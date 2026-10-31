extern int func_02008444();
extern int func_0200e62c();
extern int func_ov006_020d4e48();

void func_ov006_020d4d68(int param_1)
{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    unsigned int uVar4;
    int iVar5;
    int iVar6;

    func_02008444();
    func_0200e62c();
    uVar1 = func_02008444();
    func_ov006_020d4e48(uVar1, 1);
    *(unsigned char *)(param_1 + 0x24) = 0;
    *(unsigned char *)(param_1 + 0x25) = 0;
    *(unsigned char *)(param_1 + 0x26) = 0;
    *(unsigned char *)(param_1 + 0x28) = 0;
    *(unsigned char *)(param_1 + 0x29) = 0;
    *(unsigned char *)(param_1 + 0x2b) = 0;
    *(unsigned short *)(param_1 + 0x3d4) = 0xffff;
    *(unsigned short *)(param_1 + 0x3d6) = 0xffff;
    *(unsigned int *)(param_1 + 0x410) = 0;
    *(unsigned int *)(param_1 + 0x414) = 0;
    *(unsigned int *)(param_1 + 0x418) = 0;
    *(unsigned int *)(param_1 + 0x41c) = 0;
    *(unsigned int *)(param_1 + 0x420) = 0;
    *(unsigned int *)(param_1 + 0x424) = 0;
    *(unsigned int *)(param_1 + 0x428) = 0;
    *(unsigned int *)(param_1 + 0x42c) = 0;
    *(unsigned int *)(param_1 + 0x430) = 0;
    *(unsigned int *)(param_1 + 0x434) = 0;
    for (uVar2 = 0; uVar2 < 0x21; uVar2 = uVar2 + 1) {
        iVar5 = param_1 + uVar2 * 4;
        for (uVar3 = 0; uVar3 < 2; uVar3 = uVar3 + 1) {
            iVar6 = iVar5 + uVar3 * 0x84;
            *(unsigned int *)(iVar6 + 0x438) = 0;
            *(unsigned int *)(iVar6 + 0x540) = 0;
        }
    }
    for (uVar4 = 0; uVar4 < 2; uVar4 = uVar4 + 1) {
        *(unsigned char *)(param_1 + uVar4 + 0x32) = 0;
        *(unsigned char *)(param_1 + uVar4 + 0x34) = 0;
    }
    for (uVar2 = 0; uVar2 < 5; uVar2 = uVar2 + 1) {
        *(unsigned int *)(param_1 + uVar2 * 4 + 0x648) = 0;
    }
    *(unsigned int *)(param_1 + 0x660) = 0;
    *(unsigned int *)(param_1 + 0x65c) = 0;
    return;
}
