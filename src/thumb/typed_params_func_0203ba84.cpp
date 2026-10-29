class Object0203ba84_0 {
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
    virtual void slot14(int argument0);
};

extern "C" {
typedef int code(...);
extern unsigned long long func_0207b694(...);

void func_0203ba84(int *param_1,int param_2,int param_3,unsigned int param_4)

{
    unsigned long long uVar1;

    param_1[0x1a] = param_2;
    param_1[0x1b] = param_3;
    uVar1 = func_0207b694(param_3 - param_2,param_4);
    param_1[0x1c] = (int)uVar1;
    ((Object0203ba84_0 *)(param_1))->slot14((int)(param_2));
    return;
}
}
