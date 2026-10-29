class Object0205de1c_0 {
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
};

extern "C" {
extern char data_020af000;
typedef int code(...);
extern int func_02056978(...);
extern int func_0205e22c(...);

int * func_0205de1c(int *param_1)

{
    func_02056978();
    *param_1 = ((int)&data_020af000);
    *(unsigned char *)((int)param_1 + 0x4f) = 1;
    *(unsigned char *)(param_1 + 0x14) = 0;
    *(unsigned char *)((int)param_1 + 0x51) = 1;
    *(unsigned char *)((int)param_1 + 0x52) = 1;
    *(unsigned char *)((int)param_1 + 0x53) = 0;
    *(unsigned char *)(param_1 + 0x15) = 0;
    *(unsigned char *)((int)param_1 + 0x55) = 0;
    param_1[0x20] = 0;
    param_1[0x1c] = 0;
    param_1[0x1d] = 0;
    param_1[0x1e] = 0;
    param_1[0x1f] = 0;
    param_1[0x21] = 0;
    ((Object0205de1c_0 *)param_1)->slot10();
    func_0205e22c(param_1,0);
    return param_1;
}
}
