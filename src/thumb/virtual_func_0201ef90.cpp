class Object0201ef90_0 {
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
extern int func_02031270(...);

int * func_0201ef90(int *param_1)

{
    *param_1 = ((int)0x020a4670u);
    ((Object0201ef90_0 *)param_1)->slot5();
    func_02007c10(param_1);
    func_02031270(param_1);
    return param_1;
}
}
