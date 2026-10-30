extern int func_02018710();
extern int *func_0201bf24();
extern int func_0201c248();

unsigned int func_020194c8(int param_1)
{
    int *piVar1;
    unsigned int uVar2;
    unsigned int uVar3;

    uVar3 = 10;
    uVar2 = (unsigned int)*(unsigned short *)(*(int *)(param_1 + 0x14) + 2);
    if (((uVar2 == 0x30) || (uVar2 == 0x5f)) || ((uVar2 == 0x8f) || ((uVar2 == 0xbf || (uVar2 == 0xef))))) {
        uVar3 = 0xb;
    }
    piVar1 = func_0201bf24(*(unsigned int *)(param_1 + 0x70),uVar2,uVar3);
    if (piVar1 != (int *)0x0) {
        func_02018710(piVar1);
        func_0201c248(piVar1,0xffff7000,0x10000);
        *(unsigned char *)(param_1 + 0x5c) = 1;
    }
    return 0;
}
