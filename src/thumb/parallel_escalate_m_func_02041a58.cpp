class Object02041a58_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4(int argument0);
};

extern "C" {
extern char data_020a7c00;
extern int func_02007b70(...);
extern int func_02007bd8(...);
extern void func_02022d64(...);
extern void func_02031270(...);

unsigned int *func_02041a58(unsigned int *param_1)
{
    int iVar1;
    int *piVar2;

    *param_1 = (unsigned int)&data_020a7c00;
    if ((((unsigned int)*(unsigned char *)((int)param_1 + 0x51)) << 29) >> 31 == 0 && param_1[0x12] != 0) {
        func_02007bd8();
        iVar1 = func_02007b70();
        if (iVar1 != 0) {
            func_02007bd8();
            piVar2 = (int *)func_02007b70();
            ((Object02041a58_1 *)(piVar2))->slot4((int)(param_1[0x12]));
            param_1[0x12] = 0;
        }
    }
    func_02022d64(param_1);
    func_02031270(param_1);
    return param_1;
}
}
