class Object {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8(int);
};

extern "C" {
extern unsigned char data_020cff60[];
extern int func_02008444(...);
extern void func_020113f8(...);
extern int func_0205daa8(...);
extern unsigned int func_02011320(...);
extern void func_0202665c(...);
extern int func_0205da98(...);
}

extern "C" void func_0205e400(unsigned char *param_1)
{
    func_020113f8(func_02008444());
    param_1[0xcf] = param_1[0xcf] + (unsigned char)(data_020cff60[1] + 1);
    if (param_1[0xcf] >= 3) {
        param_1[0xcf] = 3;
    }
    if (param_1[0xcc] == 0 && param_1[0xcd] != 0 && func_0205daa8(*(int *)(param_1 + 0xc8)) != 0 && param_1[0xce] != 0) {
        param_1[0xcc] = 1;
        if (func_02011320(func_02008444()) != 2) {
            func_0202665c(func_02008444());
        }
    }
    if (func_0205da98(*(int *)(param_1 + 0xc8)) != 0) {
        ((Object *)param_1)->slot8(0);
    }
}
