class Object0205e5fc_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8(int argument0);
};

class Object0205e5fc_1 {
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
extern void func_0201001c(void *argument0);
extern void func_02051594(void *argument0);
extern unsigned int func_0205da98(int argument0);
extern unsigned char data_020cff60[];

void func_0205e5fc(unsigned char *param_1)
{
    unsigned int uVar1;

    param_1[0xd0] = param_1[0xd0] + (unsigned char)(data_020cff60[1] + 1);
    if (param_1[0xd0] >= 3) {
        param_1[0xd0] = 3;
        func_02051594(*(void **)(param_1 + 0xcc));
        ((Object0205e5fc_1 *)*(void **)(param_1 + 0xcc))->slot10();
        *(int *)(param_1 + 0xd4) = *(int *)(param_1 + 0xd4) + *(int *)(param_1 + 0xd8);
        *(int *)(param_1 + 0xd8) = *(int *)(param_1 + 0xd8) + *(int *)(param_1 + 0xdc);
        if (*(int *)(param_1 + 0xd8) < 0) {
            *(int *)(param_1 + 0xd8) = 0;
        }
        *(int *)(param_1 + 0xe0) = *(int *)(param_1 + 0xe0) + *(int *)(param_1 + 0xe4);
        *(int *)(param_1 + 0xe4) = *(int *)(param_1 + 0xe4) + *(int *)(param_1 + 0xe8);
    }
    else {
        func_0201001c(*(void **)(param_1 + 0xcc));
        ((Object0205e5fc_1 *)*(void **)(param_1 + 0xcc))->slot11();
    }
    uVar1 = func_0205da98(*(int *)(param_1 + 0xc8));
    if (uVar1 != 0) {
        ((Object0205e5fc_0 *)param_1)->slot8(0);
    }
    return;
}
}
