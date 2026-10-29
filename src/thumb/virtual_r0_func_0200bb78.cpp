class Object0200bb78_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual int slot4(int argument0);
};

extern "C" {
typedef int code(...);

unsigned int func_0200bb78(int param_1)

{


    unsigned int uVar1;

    uVar1 = ((Object0200bb78_0 *)(*(int **)(param_1 + 0x14)))->slot4((int)(*(unsigned int *)(param_1 + 8)));
    return uVar1;
}
}
