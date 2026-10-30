class Object {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8(int);
};

extern "C" void func_02056cb8(...);

extern "C" void func_02056cfc(char *param_1)
{
    signed char start;
    int total;

    func_02056cb8(param_1);
    if (*(unsigned char *)(param_1 + 0xd4) == 0 && *(short *)(param_1 + 0xc6) > 0) {
        *(short *)(param_1 + 0xc6) = *(short *)(param_1 + 0xc6) - 1;
        if (*(short *)(param_1 + 0xc6) == 0) {
            start = *(signed char *)(param_1 + 0xc8);
            *(signed char *)(param_1 + 0x44) = start;
            *(int *)(param_1 + 0xcc) = start << 12;
            if (*(signed char *)(param_1 + 0x44) <= 0) {
                ((Object *)param_1)->slot8(0);
            }
        }
        else {
            total = *(int *)(param_1 + 0xcc) + *(int *)(param_1 + 0xd0);
            *(int *)(param_1 + 0xcc) = total;
            *(signed char *)(param_1 + 0x44) = total >> 12;
        }
    }
}
