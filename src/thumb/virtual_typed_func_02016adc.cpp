class Object02016adc_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
};

class Object02016adc_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
};

class Object02016adc_2 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
};

extern "C" {
typedef int code(...);
extern int func_020151f0(...);

void func_02016adc(int param_1)

{
    int iVar1;

    iVar1 = func_020151f0();
    if ((iVar1 == 0) && (*(int **)(param_1 + 4) != (int *)0x0)) {
        ((Object02016adc_0 *)(*(int **)(param_1 + 4)))->slot2();
        ((Object02016adc_1 *)(*(int **)(param_1 + 4)))->slot3();
        ((Object02016adc_2 *)(*(int **)(param_1 + 4)))->slot4();
    }
    return;
}
}
