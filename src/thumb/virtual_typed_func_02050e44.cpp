class Object02050e44_0 {
public:
    virtual void slot0();
    virtual void slot1();
};

extern "C" {
extern char data_020abcdc;
typedef int code(...);
extern int func_02007ab4(...);
extern int func_02050e38(...);

unsigned int * func_02050e44(unsigned int *param_1)

{
    *param_1 = ((unsigned int)&data_020abcdc);
    if ((int *)param_1[0xf] != (int *)0x0) {
        ((Object02050e44_0 *)(param_1[0xf]))->slot1();
    }
    param_1[0xf] = 0;
    func_02050e38(param_1 + 1);
    func_02007ab4(param_1);
    return param_1;
}
}
