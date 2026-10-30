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
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
};

extern "C" {
extern char data_020af5c0;
extern int func_0200f70c(...);
extern int func_020569f0(...);
}

extern "C" int * func_0205f18c(int *param_1)
{
    func_020569f0(param_1);
    *param_1 = ((int)&data_020af5c0);
    *(unsigned char *)(param_1 + 0x1f) = *(unsigned char *)(param_1 + 0x1f) | 0x20;
    *(unsigned char *)((int)param_1 + 0x7d) = *(unsigned char *)((int)param_1 + 0x7d) & ~0x1;
    *(unsigned short *)((int)param_1 + 0x86) = 0;
    *(unsigned short *)(param_1 + 0x22) = 0;
    *(unsigned short *)((int)param_1 + 0x8a) = 0;
    func_0200f70c(param_1 + 0x26);
    func_0200f70c(param_1 + 0x2f);
    param_1[0x34] = 0;
    param_1[0x35] = 0;
    param_1[0x36] = 0;
    param_1[0x37] = 0;
    param_1[0x38] = 0;
    param_1[0x39] = 0;
    param_1[0x3a] = 0;
    *(unsigned short *)(param_1 + 0x3b) = 0;
    param_1[0x3c] = 0;
    param_1[0x3d] = 0;
    param_1[0x3e] = 0;
    param_1[0x29] = 0x30000;
    ((Object *)param_1)->slot10();
    return param_1;
}
