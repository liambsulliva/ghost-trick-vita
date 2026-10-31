class Pool {
public:
    virtual void v0();
    virtual void v1();
    virtual int alloc(int a, void *b);
};

extern "C" {
void func_0200a7dc(int a, int b, int c, int d);
extern char data_02083af0;

int func_0200a4a0(int param_1, int param_2, int param_3)
{
    int puVar4;
    int off;
    int iVar2;
    int iVar3;
    int iVar1;

    iVar2 = param_1 + 0x7a04 + param_3 * 0x100;
    off = param_2 * 4;
    iVar3 = *(int *)(iVar2 + off);
    if (iVar3 == 0) {
        iVar1 = 0xaa04;
        puVar4 = ((Pool *)(param_1 + iVar1))->alloc(0, &data_02083af0);
        if (puVar4 != 0) {
        *(int *)(iVar2 + off) = puVar4;
        func_0200a7dc(param_1 + *(unsigned char *)(param_1 + param_2 + param_3 * 0x40 + 0x6e02) * 0x10, param_2, param_3, puVar4);
        *(unsigned short *)(param_1 + iVar1 + 0x2a) = *(unsigned short *)(param_1 + iVar1 + 0x2a) + 1;
        *(short *)(param_1 + iVar1 + 0x28) = *(short *)(param_1 + iVar1 + 0x28) + 1;
        return puVar4;
        }
        return 0;
    }
    return iVar3;
}
}
