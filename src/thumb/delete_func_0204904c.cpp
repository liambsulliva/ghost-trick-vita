class Object0204904c_0 {
public:
    virtual ~Object0204904c_0();
};

class Object0204904c_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(int argument0);
};

extern "C" {
extern char data_020a9828;
typedef int code(...);
extern int func_02007b70(...);
extern int func_02007bd8(...);
extern int func_02042b20(...);
extern int func_02047198(...);

unsigned int * func_0204904c(unsigned int *param_1)

{
    int iVar1;
    int *piVar2;

    *param_1 = ((unsigned int)&data_020a9828);
    if (param_1[0x2e] != 0) {
        func_02042b20();
        if ((int *)param_1[0x2e] != (int *)0x0) {
            delete ((Object0204904c_0 *)(param_1[0x2e]));
            param_1[0x2e] = 0;
        }
    }
    if (param_1[0x2f] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            ((Object0204904c_1 *)(piVar2))->slot4((int)(param_1[0x2f]));
            param_1[0x2f] = 0;
        }
    }
    func_02047198(param_1);
    return param_1;
}
}
