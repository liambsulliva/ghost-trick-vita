class Object02011d74_0 {
public:
    virtual ~Object02011d74_0();
};

extern "C" {
typedef int code(...);
extern int func_02011d1c(...);

void func_02011d74(int param_1)

{
    int iVar1;
    int *piVar2;
    unsigned int uVar3;
    int iVar4;

    func_02011d1c();
    iVar1 = ((int)0x00000fa4u);
    uVar3 = 0;
    do {
        iVar4 = param_1 + uVar3 * 4;
        piVar2 = *(int **)(iVar4 + iVar1);
        if (piVar2 != (int *)0x0) {
            delete ((Object02011d74_0 *)(piVar2));
            *(unsigned int *)(iVar4 + iVar1) = 0;
        }
        uVar3 = uVar3 + 1;
    } while (uVar3 < 4);
    return;
}
}
