struct State
{
    unsigned int before[4];
    int data;
};

extern struct State data_020b18a0;
extern int *func_02000d9c();
extern void func_020055fc();
extern void func_02007a8c();
extern void func_02007948();
extern void func_0200551c();
extern void func_020055f0();

void func_02005654(unsigned int param_1)
{
    int *iVar1;
    int iVar2;
    iVar2 = (int)&data_020b18a0;
    iVar1 = func_02000d9c(*(int *)(*(int *)(iVar2 + 0x10) + 0x1580), param_1, *(int *)(*(int *)(*(int *)(*(int *)(iVar2 + 0x10) + 0x1580) + 0x98) + 0x24));
    if (iVar1 != 0 && (iVar2 = *(int *)(iVar2 + 0x10) + 0x3d28 + *(unsigned char *)((int)iVar1 + 6) * 0x3a4, (*(unsigned int *)(iVar2 + 0xe8) & 1) != 0)) {
        func_020055fc(iVar2, 0);
        func_02007a8c(*(unsigned int *)(iVar2 + 0x108), 0, 1 << *(int *)(iVar2 + 0x10c), 0);
        func_02007948();
        func_0200551c(*(unsigned char *)((int)iVar1 + 6));
        func_020055f0(*(int *)(iVar2 + 0x114), *(int *)(iVar2 + 0x11c));
        if (*(int *)(iVar2 + 0x118) != 0) {
            func_020055f0(*(int *)(iVar2 + 0x118), *(int *)(iVar2 + 0x11c));
        }
        *(unsigned char *)(iVar2 + 0xf4) = 0;
    }
}
