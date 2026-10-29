class Object0200b180_0 {
public:
    virtual void slot0();
    virtual void slot1();
};

class Object0200b180_1 {
public:
    virtual void slot0();
    virtual void slot1();
};

extern "C" {
extern char data_020a2c9c;
typedef int code(...);
extern int func_02007ab4(...);
extern int func_0200b208(...);
extern int func_02023f24(...);

unsigned int * func_0200b180(unsigned int *param_1)

{
    unsigned int uVar1;

    *param_1 = ((unsigned int)&data_020a2c9c);
    if ((int *)param_1[1] != (int *)0x0) {
        ((Object0200b180_0 *)(param_1[1]))->slot1();
    }
    param_1[1] = 0;
    if ((int *)param_1[2] != (int *)0x0) {
        ((Object0200b180_1 *)(param_1[2]))->slot1();
    }
    param_1[2] = 0;
    uVar1 = func_0200b208();
    func_02023f24(uVar1,param_1[3]);
    func_02007ab4(param_1);
    return param_1;
}
}
