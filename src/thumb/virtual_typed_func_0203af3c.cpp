class Object0203af3c_0 {
public:
    virtual void slot0();
    virtual void slot1();
};

extern "C" {
extern char data_020a6a88;
typedef int code(...);
extern int func_02010b7c(...);
extern int func_0202f17c(...);
extern int func_02038200(...);
extern int func_02056cac(...);

unsigned int * func_0203af3c(unsigned int *param_1)

{
    unsigned int uVar1;

    *param_1 = ((unsigned int)&data_020a6a88);
    if (param_1[0x33] != 0) {
        func_02038200();
        uVar1 = func_02010b7c();
        func_0202f17c(uVar1,param_1[0x33]);
        if ((int *)param_1[0x33] != (int *)0x0) {
            ((Object0203af3c_0 *)(param_1[0x33]))->slot1();
        }
    }
    func_02056cac(param_1);
    return param_1;
}
}
