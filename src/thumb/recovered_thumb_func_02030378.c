extern int func_020303b0();
extern int func_02068d98();
extern int func_02069100();
extern int func_020699cc();
extern int func_0206f7d0();

void func_02030378(void)

{
    unsigned short *puVar1;
    unsigned short uVar2;
    unsigned int uVar3;

    func_0206f7d0();
    func_020699cc();
    uVar2 = func_02068d98(0x20);
    puVar1 = ((unsigned short *)0x020cc1a8u);
    *((unsigned short *)0x020cc1a8u) = uVar2;
    uVar3 = func_020303b0(*puVar1,4);
    *(unsigned int *)(puVar1 + 2) = uVar3;
    uVar2 = func_02069100(0x20);
    puVar1[1] = uVar2;
    uVar3 = func_020303b0(puVar1[1],4);
    *(unsigned int *)(puVar1 + 4) = uVar3;
    return;
}
