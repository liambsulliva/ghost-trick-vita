class Object02014b98_0 {
public:
    virtual int slot0();
};

extern "C" {
typedef int code(...);
extern int func_02008444(...);
extern int func_02009900(...);
extern int func_0201003c(...);
extern int func_0201192c(...);
extern int func_02014b3c(...);
extern int func_02015970(...);
extern int func_0202f7e8(...);

void func_02014b98(int param_1,unsigned int param_2,unsigned int *param_3)

{
    unsigned int uVar1;
    int iVar2;

    uVar1 = func_02008444();
    func_0201192c(uVar1,0xe);
    *(unsigned int *)(param_1 + 8) = param_2;
    func_0201003c();
    func_02015970();
    iVar2 = ((Object02014b98_0 *)param_3)->slot0();
    *(int *)(param_1 + 4) = iVar2;
    if (iVar2 != 0) {
        func_02014b3c(iVar2,param_2,0);
        uVar1 = func_02009900();
        func_0202f7e8(uVar1,*(unsigned int *)(param_1 + 4),1,0);
    }
    return;
}
}
