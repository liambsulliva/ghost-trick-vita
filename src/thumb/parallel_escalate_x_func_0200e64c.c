extern void func_02007aa8();
extern void func_02012620();
extern void func_0200e608();
extern void func_0200e624();
extern void *func_02007bd8();
extern void *func_02007b70();
extern void func_0200e62c();
extern char data_020a2f94;
extern char data_02083dbc;
typedef int code();

int *func_0200e64c(int *param_1)
{
    unsigned char *base;
    void *puVar1;
    int *piVar2;
    unsigned int uVar3;
    unsigned int uVar7;
    unsigned int uVar4;
    int iVar5;
    int iVar6;

    base = (unsigned char *)param_1;
    func_02007aa8(param_1);
    *param_1 = (int)&data_020a2f94;
    func_02012620(param_1 + 1);
    uVar7 = 0;
    iVar5 = 0xe58;
    *(int *)(base + iVar5) = 0;
    *(int *)(base + iVar5 + 0x84) = 0;
    *(int *)(base + iVar5 + 0x88) = 0;
    *(int *)(base + iVar5 + 0x8c) = 0;
    *(int *)(base + iVar5 + 0x90) = 0;
    *(int *)(base + iVar5 + 0x94) = 0;
    *(int *)(base + iVar5 + 0x98) = 0;
    *(int *)(base + iVar5 + 0x9c) = 0;
    *(int *)(base + iVar5 + 0xa0) = 0;
    func_0200e608(base + iVar5 + 0xac);
    *(int *)(base + iVar5 + 0xbc) = 0;
    base[iVar5 + 0xc8] = 0;
    base[iVar5 + 0xca] = 0;
    iVar5 = 0xfb4;
    *(int *)(base + iVar5) = 0;
    *(int *)(base + iVar5 + 4) = 0;
    base[iVar5 + 0x89] = 0;
    *(int *)(base + iVar5 + 0x90) = 0;
    *(int *)(base + iVar5 + 0xe4) = 0;
    func_0200e624(base + iVar5 + 0xec);
    iVar6 = 0x10e4;
    base[iVar6] = 0;
    puVar1 = func_02007bd8();
    piVar2 = (int *)func_02007b70(puVar1);
    uVar3 = (*(code **)(*piVar2 + 8))(piVar2, 0xfff80000, &data_02083dbc);
    *(unsigned int *)(base + iVar5 - 0xd0) = uVar3;
    func_0200e62c(param_1);
    base[iVar6 - 0x48] = base[iVar6 - 0x48] & ~1;
    *(unsigned short *)(base + iVar5 + 8) = 0xfffe;
    do {
        iVar5 = uVar7 * 4;
        uVar7 = uVar7 + 1;
        *(int *)(base + iVar6 - 0x9c + iVar5) = 0;
        iVar5 = 0xfa4;
    } while (uVar7 < 4);
    uVar4 = 0;
    do {
        iVar6 = uVar4 * 4;
        uVar4 = uVar4 + 1;
        *(int *)(base + iVar5 + iVar6) = 0;
        iVar6 = 0xefc;
    } while (uVar4 < 4);
    *(int *)(base + 0xefc) = -1;
    *(int *)(base + iVar6 + 4) = -1;
    return param_1;
}
