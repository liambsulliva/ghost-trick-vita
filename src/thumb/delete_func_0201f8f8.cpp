class Object0201f8f8_0 {
public:
    virtual ~Object0201f8f8_0();
};

extern "C" {
extern char data_020a487c;
typedef int code(...);
extern int func_0201f5f8(...);

unsigned int * func_0201f8f8(unsigned int *param_1)

{
    int *piVar1;
    unsigned int uVar2;

    *param_1 = ((unsigned int)&data_020a487c);
    for (uVar2 = 0; uVar2 < (unsigned int)param_1[2]; uVar2 = uVar2 + 1) {
        piVar1 = *(int **)(param_1[3] + uVar2 * 4);
        if (piVar1 != (int *)0x0) {
            delete ((Object0201f8f8_0 *)(piVar1));
            *(unsigned int *)(param_1[3] + uVar2 * 4) = 0;
        }
    }
    func_0201f5f8(param_1);
    return param_1;
}
}
