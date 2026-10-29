class Object020413fc_0 {
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
extern unsigned char * func_0203fb00(...);

int * func_020413fc(int *param_1)

{
    int iVar1;
    unsigned int uVar2;

    func_0203fb00((unsigned char *)param_1);
    uVar2 = 0;
    *param_1 = ((int)0x020a7a44u);
    *(unsigned char *)(param_1 + 0x2f) = 0;
    do {
        iVar1 = uVar2 * 4;
        uVar2 = uVar2 + 1;
        *(int *)((int)param_1 + iVar1 + 0xc0) = 0;
    } while (uVar2 < 0xb);
    ((Object020413fc_0 *)(param_1))->slot10();
    return param_1;
}
}
