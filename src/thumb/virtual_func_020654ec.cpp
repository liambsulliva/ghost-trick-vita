class Object020654ec_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
};

extern "C" {
typedef int code(...);

void func_020654ec(int *param_1)

{
    *(unsigned char *)(param_1 + 8) = 1;
    ((Object020654ec_0 *)param_1)->slot4();
    return;
}
}
