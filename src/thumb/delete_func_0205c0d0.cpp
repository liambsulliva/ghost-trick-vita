class Object0205c0d0_0 {
public:
    virtual ~Object0205c0d0_0();
};

extern "C" {
extern char data_020af0ac;
typedef int code(...);
extern int func_020293f8(...);
extern int func_0202f644(...);

unsigned int * func_0205c0d0(unsigned int *param_1)

{
    *param_1 = ((unsigned int)&data_020af0ac);
    if ((int *)param_1[0x12] != (int *)0x0) {
        delete ((Object0205c0d0_0 *)(param_1[0x12]));
        param_1[0x12] = 0;
    }
    func_020293f8(param_1);
    func_0202f644(param_1);
    return param_1;
}
}
