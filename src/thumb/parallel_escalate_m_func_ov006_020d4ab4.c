extern signed char data_ov006_020d8e60[];
extern unsigned char data_ov006_020db4a0[];
extern int func_0202970c();
extern int func_02029708();
extern int func_ov006_020d4ba4();
extern int func_ov006_020d4bb0();
extern int func_ov006_020d4bc0();
extern int func_ov006_020d4bfc();
extern int func_ov006_020d4c20();
extern int func_ov006_020d4c2c();
extern int func_ov006_020d4c38();
extern int func_ov006_020d4c54();

void func_ov006_020d4ab4(int param_1)
{
    unsigned short uVar1;
    signed char *psVar6;
    unsigned int uVar3;
    int iVar4;
    int iVar5;

    func_0202970c();
    iVar4 = func_02029708();
    uVar1 = *(unsigned short *)(param_1 + 0x42);
    iVar5 = -(iVar4 - (uVar1 - 1));
    func_ov006_020d4ba4();
    func_ov006_020d4bb0(0, 0, iVar5);
    func_ov006_020d4bc0(0, 0, 0, 0, 0, 0, 0, 0);
    psVar6 = data_ov006_020d8e60;
    iVar4 = psVar6[2];
    if (0x1f < iVar4) {
        iVar4 = 0x1f;
    }
    func_ov006_020d4bfc(0, 0, 3, 0x33, iVar4, 0);
    func_ov006_020d4c20(0);
    func_ov006_020d4c2c((unsigned short)((unsigned int)((unsigned char *)data_ov006_020d8e60)[0] | (unsigned int)data_ov006_020db4a0[1] << 5 | (unsigned int)data_ov006_020db4a0[0] << 10));
    uVar3 = 0xfffff000u;
    func_ov006_020d4c38(uVar3, 0x1000, 0);
    func_ov006_020d4c38(uVar3, uVar3, 0);
    func_ov006_020d4c38((short)((int)(((unsigned char *)data_ov006_020d8e60)[1] * 0x1000) / 128 - 0x1000), 0x1000, 0);
    func_ov006_020d4c38((short)((int)(((unsigned char *)data_ov006_020d8e60)[1] * 0x1000) / 128 * -1), 0x1000, 0);
    func_ov006_020d4c38(0, uVar3, 0);
    func_ov006_020d4c38(0, 0x1000, 0);
    func_ov006_020d4c54();
    return;
}
