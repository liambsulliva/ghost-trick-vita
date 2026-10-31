extern signed char data_020b18a0;
extern int func_020079bc();
extern int func_020079e8();
extern int func_0207b424();
extern int func_0207ba84();
extern int func_0207bb44();
extern int func_0207bd24();
extern int func_0207c1d8();

void func_02003624(int param_1,unsigned int param_2,int param_3,int param_4,unsigned char param_5)

{
    unsigned char bVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    int iVar4;
    int iVar5;
    int slot;

    iVar5 = *(int *)(((int)&data_020b18a0) + 0x10) + ((int)0x000015bcu) + param_1 * 0x4c;
    iVar4 = *(int *)(((int)&data_020b18a0) + 0x10) + ((int)0x00001a7cu);
    bVar1 = *(unsigned char *)(iVar5 + 0x11);
    slot = iVar4 + (unsigned int)bVar1 * 8;
    if ((param_2 & 7) != 0) {
        *(unsigned char *)(iVar5 + 0xe) = param_5;
        uVar2 = func_0207b424();
        uVar3 = func_0207b424(*(unsigned char *)(slot + 6));
        uVar2 = func_0207bb44(uVar2,uVar3);
        uVar2 = func_0207c1d8(uVar2,((unsigned int)0x42fe0000u));
        func_0207bd24(0x3f000000,uVar2);
        uVar2 = func_0207ba84();
        func_020079bc(*(unsigned char *)(iVar5 + 0x30),uVar2);
    }
    if ((param_2 & 0x18) != 0) {
        if ((param_2 & 8) == 0) {
            param_3 = (unsigned int)*(unsigned char *)(iVar5 + 0x10) + param_3;
        }
        if (param_3 < 0) {
            param_3 = 0;
        }
        else if (0x7f < param_3) {
            param_3 = 0x7f;
        }
        *(signed char *)(iVar5 + 0x10) = (signed char)param_3;
    }
    if ((param_2 & 0xc0) != 0) {
        if ((param_2 & 0x40) == 0) {
            param_4 = (unsigned int)*(unsigned char *)(iVar5 + 0xf) + param_4;
        }
        if (param_4 < 0) {
            param_4 = 0;
        }
        else if (0x7f < param_4) {
            param_4 = 0x7f;
        }
        *(signed char *)(iVar5 + 0xf) = (signed char)param_4;
        func_020079e8(*(unsigned char *)(iVar5 + 0x30),param_4);
    }
    return;
}
