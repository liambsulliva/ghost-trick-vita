struct Vobj11b04 {
    virtual void v0();
    virtual void v1();
    virtual unsigned short *v2(unsigned int a, unsigned int b);
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual int v10(unsigned int a);
    virtual unsigned int v11();
};

extern "C" {
void func_02078e84(char *a, char *b);
void func_02031270(int a);
void func_0200a424(int a);
void *func_02031254(unsigned int a);
Vobj11b04 *func_0201f33c(Vobj11b04 *a, unsigned int b, unsigned int c, int d);
void func_0200e5c0(void *a);
void func_020119d0(void *a, Vobj11b04 *b);
void *func_02007bd8(void);
Vobj11b04 *func_02007b70(void *a);
void func_0207303c(unsigned short *a, unsigned short *b, unsigned int c);
void func_0200a39c(int a, int b, int c);
}

extern "C" void func_02011b04(int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4)
{
    int iVar1;
    Vobj11b04 *piVar2;
    unsigned int uVar3;
    void *puVar4;
    Vobj11b04 *piVar5;
    int iVar6;
    unsigned short *puVar7;
    unsigned short *puVar9;
    unsigned int uStack_18;
    unsigned int k;

    iVar6 = 0x1058;
    func_02078e84((char *)(param_1 + iVar6), (char *)(param_1 + 0x1c));
    iVar1 = *(int *)(param_1 + iVar6 + 0x40);
    if (iVar1 != 0) {
        func_02031270(iVar1);
        *(unsigned int *)(param_1 + iVar6 + 0x40) = 0;
    }
    func_0200a424(*(int *)(param_1 + 0xee0));
    k = 0x18;
    piVar2 = (Vobj11b04 *)func_02031254(k);
    if (piVar2 != 0) {
        piVar2 = func_0201f33c(piVar2, 0, k << 13, *(int *)(param_1 + 0xee4));
    }
    if (piVar2 != 0) {
        func_0200e5c0(&uStack_18);
        *(unsigned int *)(param_1 + 0xc) = *(unsigned int *)(param_1 + 0xc) | 0x800;
        func_020119d0(&uStack_18, piVar2);
        *(unsigned int *)(param_1 + 0xc) = 0xfffff7ffu & *(unsigned int *)(param_1 + 0xc);
        uVar3 = piVar2->v11();
        puVar9 = *(unsigned short **)(param_1 + 0xee4);
        *(unsigned int *)(param_1 + 0xc) = 0xfdffffffu & *(unsigned int *)(param_1 + 0xc);
        puVar4 = func_02007bd8();
        piVar5 = func_02007b70(puVar4);
        iVar6 = piVar5->v10(uVar3);
        if (iVar6 != 0) {
            puVar4 = func_02007bd8();
            piVar5 = func_02007b70(puVar4);
            puVar7 = piVar5->v2(-uVar3, 0x02083dbc);
            *(unsigned short **)(param_1 + 0x1098) = puVar7;
            if (puVar7 != 0) {
                func_0207303c(puVar9, puVar7, uVar3);
            }
        }
        if (piVar2 != 0) {
            piVar2->v1();
        }
    }
    func_0200a39c(*(int *)(param_1 + 0xee0), *(int *)(param_1 + 0xee4), 0x80000);
}
