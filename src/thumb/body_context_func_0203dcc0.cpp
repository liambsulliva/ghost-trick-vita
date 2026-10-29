class Object0203dcc0_0 {
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
};

class Object0203dcc0_1 {
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
    virtual int slot13();
};

extern "C" {
typedef int code(...);
extern unsigned int * func_02008444(...);
extern int func_020112b4(...);
extern int func_02022f6c(...);
extern int func_0202f64c(...);

void func_0203dcc0(unsigned char *param_1)

{
    int iVar1;
    unsigned int *puVar2;

    if (*(int *)(param_1 + 0x3c) != 0) {
        *(int *)(param_1 + 0x3c) = *(int *)(param_1 + 0x3c) + -1;
        return;
    }
    switch(param_1[4]) {
    case 0:
        param_1[4] = 1;
        *(unsigned int *)(param_1 + 0x3c) = 0x3c;
        return;
    case 1:
        ((Object0203dcc0_0 *)(*(int **)(param_1 + 0x38)))->slot12();
        param_1[4] = 2;
        return;
    case 2:
        iVar1 = ((Object0203dcc0_1 *)(*(int **)(param_1 + 0x38)))->slot13();
        if (iVar1 != 0) {
            param_1[4] = 5;
            return;
        }
        break;
    case 3:
        func_0202f64c(param_1 + 0x38);
        param_1[4] = 4;
        return;
    case 4:
        puVar2 = func_02008444();
        func_020112b4((int)puVar2);
        func_02022f6c(param_1);
    case 5:
        break;
    }
    return;
}
}
