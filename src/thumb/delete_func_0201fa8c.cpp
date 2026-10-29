class Object0201fa8c_0 {
public:
    virtual ~Object0201fa8c_0();
};

extern "C" {
typedef int code(...);
extern int func_0201f934(...);

void func_0201fa8c(int param_1,int param_2)

{
    int iVar1;
    int *piVar2;

    iVar1 = func_0201f934();
    if (iVar1 != 0) {
        piVar2 = *(int **)(*(int *)(param_1 + 0xc) + param_2 * 4);
        if (piVar2 != (int *)0x0) {
            delete ((Object0201fa8c_0 *)(piVar2));
            *(unsigned int *)(*(int *)(param_1 + 0xc) + param_2 * 4) = 0;
        }
    }
    return;
}
}
