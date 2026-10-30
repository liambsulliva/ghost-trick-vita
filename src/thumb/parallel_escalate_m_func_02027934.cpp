class Object02027934_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8(int argument0);
};

extern "C" {
extern char data_020cf8ec;
typedef int code(...);
extern int func_02008828(...);
extern int func_0200eab4(...);
extern int func_0200eab8(...);
extern int func_0200eac4(...);
extern int func_0202792c(...);
extern int func_02027c28(...);
extern int func_0202fbb0(...);
extern int func_0200eabc(...);
extern int func_020790c8(...);

void func_02027934(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    unsigned int uVar1;
    unsigned int uVar2;
    unsigned int uVar3;
    int *piVar4;
    int iVar5;
    int zero;
    unsigned char auStack_28 [16];
    unsigned int uStack_18;

    uStack_18 = param_4;
    uVar2 = func_02027c28(*(unsigned short *)(param_1 + 2));
    func_0202fbb0(auStack_28);
    zero = 0;
    while (iVar5 = func_0200eac4(auStack_28), iVar5 != 0) {
        uVar3 = func_0200eab4(auStack_28);
        iVar5 = func_02008828(uVar3,&data_020cf8ec);
        if (iVar5 != 0) {
            piVar4 = (int *)func_0200eab8(auStack_28);
            uVar3 = func_0202792c();
            iVar5 = func_020790c8(uVar2,uVar3);
            if (iVar5 == 0) {
                ((Object02027934_0 *)(piVar4))->slot8(zero);
            }
        }
        func_0200eabc(auStack_28,0);
    }
    return;
}
}
