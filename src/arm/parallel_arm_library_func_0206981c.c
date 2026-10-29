extern char data_020cffbc;
extern char data_020cffc4;
extern char data_020d0008;
extern char data_020d0078;
extern unsigned int * func_0206e684();
extern unsigned int * func_0206e9ac();
extern unsigned int * func_0206ece8();

void func_0206981c(void)

{
    unsigned int *puVar1;
    unsigned int uVar2;
    unsigned int *puVar3;
    int iVar4;
    int iVar5;

    *((unsigned int *)&data_020cffbc) = ((unsigned int)0x17101610u);
    puVar1 = ((unsigned int *)&data_020cffbc);
    puVar1[1] = 0;
    puVar1[0x12] = 2;
    uVar2 = ((unsigned int)0x60293130u);
    puVar1[0x1f] = ((unsigned int)0x32323232u);
    puVar1[0x24] = uVar2;
    uVar2 = ((unsigned int)0x002a1b19u);
    puVar1[0x29] = ((unsigned int)0x33333333u);
    puVar3 = ((unsigned int *)&data_020d0008);
    puVar1[0x2e] = uVar2;
    func_0206e9ac(puVar3);
    func_0206ece8(((unsigned int *)&data_020cffc4));
    iVar4 = ((int)0x40000200u);
    puVar1[0x20] = ((unsigned int)0x2d8b62d8u);
    iVar5 = ((int)0xc0080000u);
    puVar1[0x21] = iVar4;
    puVar1[0x22] = iVar4 + 0x3fffffff;
    uVar2 = ((unsigned int)0x4210c210u);
    puVar1[0x23] = iVar5;
    puVar1[0x25] = uVar2;
    puVar1[0x26] = uVar2;
    puVar1[0x27] = ((unsigned int)0x001f008fu);
    puVar1[0x28] = iVar5 + -0x90000;
    puVar1[0x2a] = ((unsigned int)0x00007fffu);
    uVar2 = ((unsigned int)0x800003e0u);
    puVar1[0x2b] = 0x4000001f;
    puVar1[0x2c] = uVar2;
    puVar1[0x2d] = 0x7e00 - iVar4;
    puVar1[0x38] = 0;
    puVar1[0x39] = 0;
    puVar3 = ((unsigned int *)&data_020d0078);
    puVar1[0x3a] = 0;
    func_0206e684(puVar3);
    puVar1[0x3b] = 0x1000;
    puVar1[0x3c] = 0x1000;
    puVar1[0x3d] = 0x1000;
    puVar1[0x3e] = 0;
    puVar1[0x3f] = 0;
    puVar1[0x92] = 0;
    puVar1[0x91] = 0;
    puVar1[0x90] = 0;
    puVar1[0x95] = 0;
    puVar1[0x93] = 0;
    puVar1[0x94] = 0x1000;
    puVar1[0x97] = 0;
    puVar1[0x96] = 0;
    puVar1[0x98] = 0xfffff000;
    return;
}
