class Object02046a74_0 {
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
};

class Object02046a74_1 {
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
extern int func_02010b54(...);
extern int func_02027518(...);

void func_02046a74(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = func_02027518(((unsigned int)0x00001021u));
    if ((iVar1 == 0) && (iVar1 = func_02010b54(*(unsigned int *)(param_1 + 0x28)), iVar1 != 0)) {
        uVar2 = 0;
        do {
            ((Object02046a74_0 *)(*(int **)(param_1 + uVar2 * 4 + 0x2c)))->slot10();
            uVar2 = uVar2 + 1;
        } while (uVar2 < 4);
        return;
    }
    uVar2 = 0;
    do {
        ((Object02046a74_1 *)(*(int **)(param_1 + uVar2 * 4 + 0x2c)))->slot11();
        uVar2 = uVar2 + 1;
    } while (uVar2 < 4);
    return;
}
}
