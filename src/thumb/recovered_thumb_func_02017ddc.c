extern int func_02008444();
extern int func_020120b0();

void func_02017ddc(int param_1)

{
    unsigned short uVar1;
    unsigned int uVar2;
    unsigned short uVar3;
    unsigned short uVar4;

    if (*(char *)(param_1 + 0x93) == '\x01') {
        uVar2 = func_02008444();
        uVar1 = *(unsigned short *)(param_1 + 0x94);
        uVar3 = 0x100;
        uVar4 = 0;
    }
    else {
        if (*(char *)(param_1 + 0x93) != '\x02') goto code_r0x02017e18;
        uVar2 = func_02008444();
        uVar1 = *(unsigned short *)(param_1 + 0x94);
        uVar3 = *(unsigned short *)(param_1 + 0x96);
        uVar4 = *(unsigned short *)(param_1 + 0x98);
    }
    func_020120b0(uVar2,uVar1,uVar3,uVar4);
code_r0x02017e18:
    *(unsigned char *)(param_1 + 0x93) = 0;
    return;
}
