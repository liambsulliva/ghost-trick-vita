class Object02041430_0 {
public:
    virtual ~Object02041430_0();
};

extern "C" {
extern char data_020a7a44;
typedef int code(...);
extern int func_0203fb14(...);

unsigned int * func_02041430(unsigned int *param_1)

{
    unsigned int uVar1;

    *param_1 = ((unsigned int)&data_020a7a44);
    for (uVar1 = 0; uVar1 < *(unsigned char *)(param_1 + 0x2f); uVar1 = uVar1 + 1) {
        if ((int *)param_1[uVar1 + 0x30] != (int *)0x0) {
            delete ((Object02041430_0 *)(param_1[uVar1 + 0x30]));
            param_1[uVar1 + 0x30] = 0;
        }
    }
    func_0203fb14(param_1);
    return param_1;
}
}
