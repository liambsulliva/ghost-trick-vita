class Object02023df4_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual int slot5();
};

extern "C" {
typedef int code(...);
extern int func_02023e28(...);

unsigned int func_02023df4(int param_1,unsigned int param_2)

{
    unsigned int uVar1;
    unsigned int uVar2;
    int *piVar3;

    uVar2 = ((unsigned int)0x00000000u);
    if (param_2 != 0) {
        piVar3 = *(int **)(param_1 + (param_2 & 0x3ff) * 4 + 0x20);
        uVar1 = func_02023e28(piVar3);
        if ((((unsigned int)0xfffffc00u) & param_2) == uVar1) {
            uVar2 = ((Object02023df4_0 *)piVar3)->slot5();
        }
    }
    return uVar2;
}
}
