class Object0201e554_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual int slot6(int argument0,int argument1);
};

extern "C" {
typedef int code(...);

unsigned int func_0201e554(int param_1,int param_2)

{


    unsigned int uVar1;


    uVar1 = ((Object0201e554_0 *)(*(int **)(param_1 + 0x10)))->slot6((int)(*(int *)(param_1 + 4) + param_2),(int)(0));
    return uVar1;
}
}
