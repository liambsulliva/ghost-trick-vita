class Object0203921c_0 {
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
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
};

extern "C" {
typedef int code(...);
extern int func_0203927c(...);

void func_0203921c(int *param_1)

{
    ((Object0203921c_0 *)(param_1))->slot16();
    if ((*(signed char *)((int)param_1 + 0xde) == -1) || (*(signed char *)((int)param_1 + 0xde) == '\x05')) {
        func_0203927c((unsigned char *)param_1);
    }
    *(unsigned char *)(param_1 + 4) = 4;
    return;
}
}
