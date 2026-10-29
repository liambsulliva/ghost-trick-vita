class Object02023cc0_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
};

extern "C" {
typedef int code(...);
extern int func_02007ab4(...);

int * func_02023cc0(int *param_1)

{
    *param_1 = ((int)0x020a4a14u);
    ((Object02023cc0_0 *)param_1)->slot6();
    func_02007ab4(param_1);
    return param_1;
}
}
