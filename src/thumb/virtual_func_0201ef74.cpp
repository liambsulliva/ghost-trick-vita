class Object0201ef74_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
};

extern "C" {
typedef int code(...);
extern int func_02007c10(...);

int * func_0201ef74(int *param_1)

{
    *param_1 = ((int)0x020a4670u);
    ((Object0201ef74_0 *)param_1)->slot5();
    func_02007c10(param_1);
    return param_1;
}
}
