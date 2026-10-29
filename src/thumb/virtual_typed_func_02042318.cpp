class Object02042318_0 {
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
extern char data_020a7e18;
extern char data_020ce5c4;
extern char data_020ce5fc;
typedef int code(...);
extern int func_02023d00(...);
extern int func_02023d80(...);
extern int func_020426b8(...);

int * func_02042318(int *param_1)

{
    *param_1 = ((int)&data_020a7e18);
    ((Object02042318_0 *)(param_1))->slot6();
    if (param_1 != ((int *)&data_020ce5fc)) {
        if ((int *)*((int *)&data_020ce5c4) == param_1) {
            *((int *)&data_020ce5c4) = param_1[7];
        }
        func_020426b8(param_1);
    }
    func_02023d00(param_1);
    func_02023d80(param_1);
    return param_1;
}
}
