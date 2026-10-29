class Object02017e20_0 {
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
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual int slot19();
};

extern "C" {
typedef int code(...);
extern int func_0201003c(...);
extern int func_02015990(...);
extern int func_020191e4(...);

void func_02017e20(int *param_1)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = ((Object02017e20_0 *)param_1)->slot19();
    if (iVar1 == 0) {
        *(unsigned char *)((int)param_1 + 0x91) = 1;
        func_020191e4(param_1,0);
        return;
    }
    *(unsigned char *)((int)param_1 + 0x92) = 0;
    uVar2 = func_0201003c();
    func_02015990(uVar2,0);
    return;
}
}
