class Object0200c7a4_0 {
public:
    virtual ~Object0200c7a4_0();
};

extern "C" {
typedef int code(...);

void func_0200c7a4(int param_1,int param_2)

{
    int *piVar1;

    param_1 = param_1 + ((int)0x00001518u);
    piVar1 = *(int **)(param_1 + param_2 * 4);
    if (piVar1 != (int *)0x0) {
        delete ((Object0200c7a4_0 *)(piVar1));
        *(unsigned int *)(param_1 + param_2 * 4) = 0;
    }
    return;
}
}
