class Object0201587c_0 {
public:
    virtual ~Object0201587c_0();
};

class Object0201587c_1 {
public:
    virtual ~Object0201587c_1();
};

extern "C" {
typedef int code(...);
extern int func_0202f64c(...);

int * func_0201587c(int *param_1)

{
    if ((int *)*param_1 != (int *)0x0) {
        delete ((Object0201587c_0 *)(*param_1));
        *param_1 = 0;
    }
    if ((int *)param_1[1] != (int *)0x0) {
        delete ((Object0201587c_1 *)(param_1[1]));
        param_1[1] = 0;
    }
    func_0202f64c(param_1 + 2);
    func_0202f64c(param_1 + 3);
    return param_1;
}
}
