struct VTarget {
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13(int value);
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17(int value);
};

extern "C" {
unsigned int func_0202b258(unsigned int a, unsigned int b, unsigned int c);
int func_0202bf50(int entry);
int func_0202c030(int entry);
int func_0202c034(int entry);
}

extern "C" void func_0202bf54(int param_1, VTarget *param_2, int param_3, int param_4)
{
    int iVar1;
    int iVar2;

    if (param_4 == -1) {
        param_4 = func_0202b258(param_3, *(unsigned int *)(*(int *)(param_1 + 0x80) + 0x1e8),
                                *(unsigned int *)(*(int *)(param_1 + 0x80) + 0x1ec));
    }
    iVar2 = param_4 * 0x10;
    iVar1 = func_0202bf50(*(int *)(*(int *)(param_1 + 0x80) + 0x1e8) + iVar2);
    if (iVar1 == 2) {
        iVar1 = func_0202c030(*(int *)(*(int *)(param_1 + 0x80) + 0x1e8) + iVar2);
        param_2->slot13(iVar1);
        iVar1 = func_0202c034(*(int *)(*(int *)(param_1 + 0x80) + 0x1e8) + iVar2);
        if (iVar1 != 0xffff) {
            iVar2 = *(int *)(*(int *)(param_1 + 0x80) + 0x1f0);
            if (iVar2 != 0) {
                param_2->slot17(iVar2 + iVar1 * 0x6c);
                return;
            }
            param_2->slot17(*(int *)(*(int *)(*(int *)(param_1 + 0x80) + 500) + 0x18) + iVar1 * 0x6c);
            return;
        }
    }
    else {
        if (iVar1 == 7) {
            iVar1 = func_0202c030(*(int *)(*(int *)(param_1 + 0x80) + 0x1e8) + iVar2);
            param_2->slot13(iVar1);
            return;
        }
        if (iVar1 == 0x27) {
            iVar1 = func_0202c030(*(int *)(*(int *)(param_1 + 0x80) + 0x1e8) + iVar2);
            param_2->slot13(iVar1);
        }
    }
}
