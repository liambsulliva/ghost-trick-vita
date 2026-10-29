class Object0201dfc8_0 {
public:
    virtual ~Object0201dfc8_0();
};

extern "C" {
typedef int code(...);

int * func_0201dfc8(int *param_1)

{
    if ((int *)*param_1 != (int *)0x0) {
        delete ((Object0201dfc8_0 *)(*param_1));
        *param_1 = 0;
    }
    return param_1;
}
}
