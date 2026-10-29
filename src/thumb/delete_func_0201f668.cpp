class Object0201f668_0 {
public:
    virtual ~Object0201f668_0();
};

extern "C" {
extern char data_020a4860;
typedef int code(...);
extern int func_02007ab4(...);
extern int func_0201f6e8(...);

unsigned int * func_0201f668(unsigned int *param_1)

{
    *param_1 = ((unsigned int)&data_020a4860);
    if ((int *)param_1[2] != (int *)0x0) {
        delete ((Object0201f668_0 *)(param_1[2]));
        param_1[2] = 0;
    }
    func_02007ab4(param_1);
    func_0201f6e8(param_1);
    return param_1;
}
}
