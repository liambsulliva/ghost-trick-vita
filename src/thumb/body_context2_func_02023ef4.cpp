class Object02023ef4_0 {
public:
    virtual void slot0();
    virtual void slot1();
};

extern "C" {
typedef int code(...);
extern int func_0200cbd0(...);

void func_02023ef4(int param_1)

{
    unsigned int uVar1;
    int *piVar2;
    unsigned int uVar3;

    piVar2 = (int *)(param_1 + 0x20);
    uVar3 = 0;
    do {
        if ((*piVar2 != 0) && (uVar1 = func_0200cbd0(*piVar2), uVar1 != 0)) {
            if ((int *)*piVar2 != (int *)0x0) {
                ((Object02023ef4_0 *)(*piVar2))->slot1();
            }
            *piVar2 = 0;
        }
        uVar3 = uVar3 + 1;
        piVar2 = piVar2 + 1;
    } while (uVar3 < 0xa0);
    return;
}
}
