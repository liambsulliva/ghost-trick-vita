class Object {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
};

extern "C" {
extern int data_020aeac4;
extern void func_0202f524(...);
extern void func_02042718(...);
}

extern "C" char *func_0205b420(char *param_1,int param_2,unsigned char param_3)
{
    func_0202f524(param_1);
    *(int *)param_1 = (int)&data_020aeac4;
    *(int *)(param_1 + 0x28) = 0;
    *(int *)(param_1 + 0x2c) = 0;
    *(int *)(param_1 + 0x30) = 0;
    *(int *)(param_1 + 0x34) = 0;
    func_02042718(param_1 + 0x38);
    *(int *)(param_1 + 0x8c) = 0;
    *(int *)(param_1 + 0x90) = 0x1000;
    *(int *)(param_1 + 0x94) = 0x266;
    *(short *)(param_1 + 0x98) = 0;
    *(short *)(param_1 + 0x9a) = 0;
    *(signed char *)(param_1 + 0x9c) = (signed char)param_2;
    *(signed char *)(param_1 + 0x9d) = (signed char)param_2;
    *(signed char *)(param_1 + 0x9e) = -1;
    *(unsigned char *)(param_1 + 0x9f) = 0;
    *(unsigned char *)(param_1 + 0xa0) = 0;
    *(unsigned char *)(param_1 + 0xa1) = 0;
    *(unsigned char *)(param_1 + 0xa2) = param_3;
    if (*(unsigned char *)(param_1 + 0xa2) != 0) {
        ((Object *)param_1)->slot5();
    }
    return param_1;
}
