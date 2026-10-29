class Object0203ddc4_0 {
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
};

extern "C" {
typedef int code(...);

unsigned int func_0203ddc4(int param_1)

{


    if (*(int **)(param_1 + 0x38) != (int *)0x0) {
        ((Object0203ddc4_0 *)(*(int **)(param_1 + 0x38)))->slot14();
    }
    *(unsigned char *)(param_1 + 4) = 3;
    *(unsigned char *)(param_1 + 5) = 1;
    return 1;
}
}
