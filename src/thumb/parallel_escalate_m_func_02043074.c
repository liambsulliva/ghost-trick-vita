extern int func_0201003c();
extern int func_02015b58();
extern int func_02043814();
extern int func_020438a0();
extern int func_02043998();

void func_02043074(int param_1,int param_2,unsigned int param_3,int param_4)
{
    int iVar1;
    int iVar3;
    int iVar5;
    unsigned int i;

    iVar5 = 0x100;
    for (i = 0; i < 5; i++) {
        if (i > 3) {
            *(unsigned short *)(param_1 + i * 16 + 0x54) = iVar5;
            *(unsigned short *)(param_1 + i * 16 + 0x56) = 0xffff;
        }
        else {
            iVar3 = param_1 + i * 16;
            iVar1 = func_0201003c(i * 16);
            if (func_02015b58(iVar1, *(unsigned short *)(iVar3 + 0x56)) == 0) {
                *(unsigned short *)(iVar3 + 0x54) = iVar5;
                *(unsigned short *)(iVar3 + 0x56) = 0xffff;
            }
            else {
                iVar1 = func_02043814(param_1, *(unsigned short *)(iVar3 + 0x56), *(unsigned short *)(iVar3 + 0x54));
                if (iVar1 != 0) {
                    *(int *)(iVar3 + 0x48) = iVar1;
                }
                iVar1 = func_020438a0(param_1, *(unsigned short *)(iVar3 + 0x56), *(unsigned short *)(iVar3 + 0x54));
                if (iVar1 != 0) {
                    *(int *)(iVar3 + 0x4c) = iVar1;
                }
            }
        }
    }
    iVar1 = func_02043998(param_1, 0);
    if (iVar1 != 0) {
        *(int *)(param_1 + 0x44) = iVar1;
    }
}
