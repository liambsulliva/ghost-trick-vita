class Object02052308_0 {
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
extern void func_02052354(...);
extern int func_02052330(...);
extern void func_02052340(...);

void func_02052308(int *param_1)
{
    int iVar1;

    func_02052354((unsigned char *)param_1);
    iVar1 = func_02052330((unsigned char *)param_1);
    if (iVar1 != 0) {
        ((Object02052308_0 *)(param_1))->slot8(0);
        return;
    }
    func_02052340((int)param_1);
}
}
