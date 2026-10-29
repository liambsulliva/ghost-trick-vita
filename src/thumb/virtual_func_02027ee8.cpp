class Object02027ee8_0 {
public:
    virtual int slot0();
};

extern "C" {
typedef int code(...);
extern int func_02009900(...);
extern int func_0201ee6c(...);
extern int func_020265c4(...);
extern int func_0202f7e8(...);

unsigned int func_02027ee8(int param_1)

{
    unsigned int uVar1;
    unsigned int *puVar2;
    unsigned int uVar3;

    uVar1 = func_020265c4(*(int *)(param_1 + 4) + *(int *)(param_1 + 8) * 2 + 2);
    puVar2 = (unsigned int *)func_0201ee6c(uVar1,((unsigned int)0x020bbbb4u));
    if (puVar2 != (unsigned int *)0x0) {
        uVar1 = func_02009900();
        uVar3 = ((Object02027ee8_0 *)puVar2)->slot0();
        func_0202f7e8(uVar1,uVar3,4,0);
    }
    *(int *)(param_1 + 8) = *(int *)(param_1 + 8) + 2;
    return 0;
}
}
