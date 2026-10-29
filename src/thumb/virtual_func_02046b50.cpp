class Object02046b50_0 {
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

void func_02046b50(int *param_1)

{
    *(unsigned char *)(param_1 + 0x11) = 1;
    param_1[0xe] = 0;
    ((Object02046b50_0 *)param_1)->slot11();
    return;
}
}
