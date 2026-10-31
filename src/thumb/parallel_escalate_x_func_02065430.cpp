class ObjectLl {
public:
    virtual void slot0();
    virtual void slot1();
};

extern "C" {
extern int func_020654dc(...);
extern long long func_020654e4(...);
extern void func_02065524(int *, long long);
extern int func_02064854(...);
extern int func_02065328(...);
extern void func_020654ec(...);

void func_02065430(unsigned char *param_1, int *param_2, int param_3)
{
    long long lVar1;
    long long lVar5;
    int iVar2;

    if (param_2 == 0) {
        return;
    }
    iVar2 = func_020654dc(param_2);
    if (iVar2 != 0) {
        return;
    }
    lVar5 = func_020654e4(param_2);
    lVar1 = lVar5 + *(int *)(param_1 + 0x30);
    func_02065524(param_2, lVar1);
    iVar2 = func_02064854((int)lVar5 + *(int *)(param_1 + 0x30));
    if (iVar2 < 0x100) {
        return;
    }
    iVar2 = 0;
    if (param_3 == 0) {
        iVar2 = func_02065328(param_1, param_2, param_1 + 0x98, param_1 + 0x99);
    }
    else if (param_3 == 1) {
        iVar2 = func_02065328(param_1, param_2, param_1 + 0x9a, param_1 + 0x9b);
    }
    if (iVar2 != 0) {
        ((ObjectLl *)param_2)->slot0();
        ((ObjectLl *)param_2)->slot1();
        lVar1 = lVar5 + *(int *)(param_1 + 0x30) + -0x200000;
        func_02065524(param_2, lVar1);
    }
    else {
        func_020654ec(param_2);
    }
}
}
