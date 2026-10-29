class Object0205b3b4_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
};

class Object0205b3b4_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
};

extern "C" {
typedef int code(...);
extern unsigned int * func_02008444(...);
extern int func_0205b020(...);
extern int func_0205b3a4(...);

void func_0205b3b4(int param_1,int param_2)

{
    unsigned int *puVar1;
    int *piVar2;

    if (param_2 == *(signed char *)(param_1 + 0x120)) {
        puVar1 = func_02008444();
        piVar2 = (int *)func_0205b020((unsigned char *)puVar1,param_2);
        if (piVar2 != (int *)0x0) {
            ((Object0205b3b4_0 *)(piVar2))->slot5();
            return;
        }
    }
    puVar1 = func_02008444();
    piVar2 = (int *)func_0205b3a4((unsigned char *)puVar1,param_2);
    if (piVar2 != (int *)0x0) {
        ((Object0205b3b4_1 *)(piVar2))->slot5();
    }
    return;
}
}
