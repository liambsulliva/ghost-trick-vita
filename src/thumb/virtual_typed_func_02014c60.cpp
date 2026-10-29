class Object02014c60_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
};

class Object02014c60_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
};

extern "C" {
extern char data_020a3494;
typedef int code(...);
extern int func_02008444(...);
extern int func_02014cb8(...);
extern int func_0202f5e4(...);
extern int func_0202f64c(...);
extern int func_02055e6c(...);

unsigned int * func_02014c60(unsigned int *param_1)

{
    *param_1 = ((unsigned int)&data_020a3494);
    func_0202f64c(param_1 + 0x11);
    func_0202f64c(param_1 + 0x13);
    if (param_1[0x12] != 0) {
        func_02055e6c();
    }
    if ((int *)param_1[0x10] != (int *)0x0) {
        ((Object02014c60_0 *)(param_1[0x10]))->slot11();
        ((Object02014c60_1 *)(param_1[0x10]))->slot12();
    }
    func_02008444();
    func_02014cb8();
    func_0202f5e4(param_1);
    return param_1;
}
}
