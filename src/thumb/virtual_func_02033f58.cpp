class Object02033f58_0 {
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
extern int func_0202fe34(...);

int * func_02033f58(int *param_1)

{
    func_0202fe34();
    *param_1 = ((int)0x020a58a4u);
    param_1[0x2f] = -1;
    *(unsigned char *)(param_1 + 0x30) = 0;
    *(unsigned char *)((int)param_1 + 0xc1) = 0;
    ((Object02033f58_0 *)param_1)->slot10();
    return param_1;
}
}
