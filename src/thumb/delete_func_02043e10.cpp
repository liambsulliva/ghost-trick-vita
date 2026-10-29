class Object02043e10_0 {
public:
    virtual ~Object02043e10_0();
};

class Object02043e10_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(int argument0);
};

extern "C" {
extern char data_020a8170;
typedef int code(...);
extern int func_02007b70(...);
extern int func_02007bd8(...);
extern int func_0202f644(...);
extern int func_02042774(...);
extern int func_02042b20(...);
extern int func_0204398c(...);

unsigned int * func_02043e10(unsigned int *param_1)

{
    int iVar1;
    int *piVar2;

    *param_1 = ((unsigned int)&data_020a8170);
    if ((int *)param_1[0x34] != (int *)0x0) {
        delete ((Object02043e10_0 *)(param_1[0x34]));
        param_1[0x34] = 0;
    }
    if (param_1[0x35] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            ((Object02043e10_1 *)(piVar2))->slot4((int)(param_1[0x35]));
            param_1[0x35] = 0;
        }
    }
    func_02042b20(param_1 + 0x36);
    func_02042774(param_1 + 0x36);
    func_0204398c(param_1);
    func_0202f644(param_1);
    return param_1;
}
}
