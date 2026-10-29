class Object02050890_0 {
public:
    virtual void slot0();
    virtual void slot1();
};

extern "C" {
extern char data_020abcf8;
typedef int code(...);
extern int func_02007ab4(...);
extern int func_0200db44(...);
extern int func_02050878(...);

unsigned int * func_02050890(unsigned int *param_1)

{
    *param_1 = ((unsigned int)&data_020abcf8);
    if ((int *)param_1[0x15] != (int *)0x0) {
        ((Object02050890_0 *)(param_1[0x15]))->slot1();
    }
    param_1[0x15] = 0;
    func_02050878(param_1 + 5);
    func_0200db44(param_1 + 1);
    func_02007ab4(param_1);
    return param_1;
}
}
