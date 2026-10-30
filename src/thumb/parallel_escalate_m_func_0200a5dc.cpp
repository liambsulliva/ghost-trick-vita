class Counter0200a5dc {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual int slot10(int value);
};

extern "C" {
int func_0200a5dc(char *param_1)
{
    int ok;

    ok = 0;
    if (((Counter0200a5dc *)(param_1 + 0xaa04))->slot10(0) == 0 || *(short *)(param_1 + 0xaa2c) >= *(unsigned char *)(param_1 + 0xaa3e) * *(unsigned char *)(param_1 + 0xaa3f)) {
        ok = 1;
    }
    return ok;
}
}
