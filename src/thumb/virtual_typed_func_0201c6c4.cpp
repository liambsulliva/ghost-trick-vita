class Object0201c6c4_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual int slot5();
};

class Object0201c6c4_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual int slot6();
};

class Object0201c6c4_2 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual int slot3();
};

class Object0201c6c4_3 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual int slot4();
};

extern "C" {
typedef int code(...);

void func_0201c6c4(int *param_1,int *param_2)

{
    unsigned char uVar1;

    *param_1 = (int)param_2;
    if (param_2 != (int *)0x0) {
        uVar1 = ((Object0201c6c4_0 *)(param_2))->slot5();
        *(unsigned char *)((int)param_1 + 0x22) = uVar1;
        uVar1 = ((Object0201c6c4_1 *)(*param_1))->slot6();
        *(unsigned char *)((int)param_1 + 0x23) = uVar1;
        uVar1 = ((Object0201c6c4_2 *)(*param_1))->slot3();
        *(unsigned char *)(param_1 + 9) = uVar1;
        uVar1 = ((Object0201c6c4_3 *)(*param_1))->slot4();
        *(unsigned char *)((int)param_1 + 0x25) = uVar1;
    }
    *(unsigned char *)((int)param_1 + 0x36) = 0;
    return;
}
}
