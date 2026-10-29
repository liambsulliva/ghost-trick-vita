class Object0203cdec_0 {
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

extern "C" {
extern char data_020ce7a0;
typedef int code(...);
extern int func_02009900(...);
extern int func_0202f7e8(...);
extern int func_0203ce3c(...);
extern int func_0203ce44(...);
extern int func_020465c4(...);
extern int func_020467fc(...);

void func_0203cdec(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = func_020465c4(((unsigned int)&data_020ce7a0));
    *(int *)(param_1 + 0x20) = iVar1;
    if (iVar1 != 0) {
        func_020467fc(iVar1,0x60);
        func_0203ce3c(*(unsigned int *)(param_1 + 0x20),0,0x4e);
        func_0203ce3c(*(unsigned int *)(param_1 + 0x20),1,0xffffffcf);
        ((Object0203cdec_0 *)(*(int **)(param_1 + 0x20)))->slot10();
        func_0203ce44(*(unsigned int *)(param_1 + 0x20));
        uVar2 = func_02009900();
        func_0202f7e8(uVar2,*(unsigned int *)(param_1 + 0x20),1,0);
    }
    return;
}
}
