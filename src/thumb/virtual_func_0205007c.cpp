class Object0205007c_0 {
public:
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
};

extern "C" {
typedef int code(...);

void func_0205007c(int param_1)

{
    int *piVar1;
    int iVar2;

    iVar2 = 0;
    do {
        piVar1 = *(int **)(param_1 + iVar2 * 4 + 0x48);
        if (piVar1 != (int *)0x0) {
            ((Object0205007c_0 *)piVar1)->slot11();
        }
        iVar2 = iVar2 + 1;
    } while (iVar2 < 0xc);
    return;
}
}
