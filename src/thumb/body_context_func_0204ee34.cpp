class Object0204ee34_0 {
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
typedef int code(...);
extern unsigned int * func_0200aca8(...);
extern unsigned int * func_02056ac8(...);

int * func_0204ee34(int *param_1)

{
    func_02056ac8(param_1);
    *param_1 = ((int)0x020ab824u);
    *(unsigned char *)((int)param_1 + 0xc5) = 0;
    *(unsigned char *)((int)param_1 + 0xc6) = 0;
    *(signed char *)((int)param_1 + 199) = -1;
    *(unsigned char *)(param_1 + 0x32) = 0;
    *(unsigned char *)(param_1 + 0x38) = 0;
    *(unsigned char *)((int)param_1 + 0xe1) = 0;
    *(unsigned char *)((int)param_1 + 0xe2) = 0;
    *(signed char *)((int)param_1 + 0xe3) = -1;
    func_0200aca8(param_1 + 0x39);
    ((Object0204ee34_0 *)(param_1))->slot10();
    return param_1;
}
}
