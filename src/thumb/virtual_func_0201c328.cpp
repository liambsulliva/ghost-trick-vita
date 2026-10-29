class Object0201c328_0 {
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
    virtual void slot12();
};

extern "C" {
typedef int code(...);
extern int func_0201c384(...);

void func_0201c328(int *param_1)

{
    int iVar1;

    ((Object0201c328_0 *)param_1)->slot12();
    func_0201c384(param_1);
    if ((0 < param_1[0xd]) && (iVar1 = param_1[0xd] - param_1[0xc], param_1[0xd] = iVar1, iVar1 < 0))
    {
        param_1[0xd] = 0;
    }
    return;
}
}
