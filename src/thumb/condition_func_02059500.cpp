class Object02059500_0 {
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
typedef int code(...);
extern int func_0200b598(...);
extern int func_0200ba28(...);

void func_02059500(int *param_1)

{
    unsigned int uVar1;

    *(unsigned char *)((int)param_1 + 0xde) = *(unsigned char *)((int)param_1 + 0xde) & ~0x10;
    ((Object02059500_0 *)(param_1))->slot10();
    uVar1 = func_0200ba28((unsigned char *)(param_1 + 10));
    func_0200b598((int)(param_1 + 10),uVar1,3);
    return;
}
}
