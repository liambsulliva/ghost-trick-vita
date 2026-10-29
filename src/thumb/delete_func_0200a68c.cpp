class Object0200a68c_0 {
public:
    virtual ~Object0200a68c_0();
};

extern "C" {
typedef int code(...);

int * func_0200a68c(int *param_1)

{
    if ((int *)*param_1 != (int *)0x0) {
        delete ((Object0200a68c_0 *)(*param_1));
        *param_1 = 0;
    }
    return param_1;
}
}
