class Object020594cc_0 {
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
extern int func_0200bbd0(...);

void func_020594cc(int *param_1)

{
    *(unsigned char *)((int)param_1 + 0xde) = *(unsigned char *)((int)param_1 + 0xde) | 0x10;
    *(unsigned char *)((int)param_1 + 0xde) = *(unsigned char *)((int)param_1 + 0xde) & ~0x8;
    ((Object020594cc_0 *)(param_1))->slot11();
    func_0200bbd0((int)(param_1 + 10));
    return;
}
}
