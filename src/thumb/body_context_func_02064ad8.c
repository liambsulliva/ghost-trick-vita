extern unsigned char * func_02009c1c();
extern int func_0202f1bc();

void func_02064ad8(int param_1,int param_2,int param_3)

{
    unsigned int *puVar1;
    int iVar2;
    unsigned int uVar3;

    uVar3 = 0x8000;
    if (param_2 + 0x8000U > *(unsigned int *)(param_1 + 0x40)) {
        uVar3 = *(unsigned int *)(param_1 + 0x40) - param_2;
    }
    iVar2 = *(int *)(param_1 + 0x38);
    if (iVar2 != 0) {
        if (param_3 == 0) {
            puVar1 = (unsigned int *)func_02009c1c();
            func_0202f1bc(puVar1,iVar2 + param_2,0,uVar3,0xb);
            return;
        }
        if (param_3 == 1) {
            puVar1 = (unsigned int *)func_02009c1c();
            func_0202f1bc(puVar1,iVar2 + param_2,0,uVar3,0x17);
        }
    }
    return;
}
