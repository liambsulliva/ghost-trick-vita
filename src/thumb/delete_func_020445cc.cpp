class Object020445cc_0 {
public:
    virtual ~Object020445cc_0();
};

class Object020445cc_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(int argument0);
};

extern "C" {
extern char data_020a844c;
typedef int code(...);
extern int func_02007b70(...);
extern int func_02007bd8(...);
extern int func_02008444(...);
extern int func_02031b04(...);
extern int func_0204454c(...);

unsigned int * func_020445cc(unsigned int *param_1)

{
    unsigned int uVar1;
    int iVar2;
    int *piVar3;

    *param_1 = ((unsigned int)&data_020a844c);
    uVar1 = func_02008444();
    func_02031b04(uVar1,param_1 + 0x36);
    if ((int *)param_1[0x37] != (int *)0x0) {
        delete ((Object020445cc_0 *)(param_1[0x37]));
        param_1[0x37] = 0;
    }
    if (param_1[0x40] != 0) {
        func_02007bd8();
        iVar2 = func_02007b70();
        if (iVar2 != 0) {
            func_02007bd8();
            piVar3 = (int *)func_02007b70();
            ((Object020445cc_1 *)(piVar3))->slot4((int)(param_1[0x40]));
            param_1[0x40] = 0;
        }
    }
    func_0204454c(param_1);
    return param_1;
}
}
