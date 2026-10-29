class Object0201bfec_0 {
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
extern int func_0201bd78(...);
extern int func_0201bdd4(...);

void func_0201bfec(int param_1)

{
    int iVar1;
    unsigned int *puVar2;
    unsigned int uVar3;
    unsigned int uVar4;

    iVar1 = func_0201bd78(param_1 + 4);
    if (iVar1 != 0) {
        for (uVar4 = 0; uVar3 = func_0201bd78(param_1 + 4), uVar4 < uVar3; uVar4 = uVar4 + 1) {
            puVar2 = (unsigned int *)func_0201bdd4(param_1 + 4,uVar4);
            ((Object0201bfec_0 *)(*puVar2))->slot11();
        }
    }
    return;
}
}
