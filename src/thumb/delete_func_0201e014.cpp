class Object0201e014_0 {
public:
    virtual ~Object0201e014_0();
};

extern "C" {
typedef int code(...);

void func_0201e014(int *param_1)

{
    if ((int *)*param_1 != (int *)0x0) {
        delete ((Object0201e014_0 *)(*param_1));
        *param_1 = 0;
    }
    return;
}
}
