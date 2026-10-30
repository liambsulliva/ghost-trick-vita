class Actor {
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
class Part {
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
    virtual void slot13(int x, int y, int z);
};
extern "C" {
extern int data_020ce758;
extern int func_02029640(int *object);
extern int func_0202ff58(int object);
void func_020466f0(int *param_1)

{
    int iVar1;

    iVar1 = func_0202ff58((int)param_1);
    {
        int getter;
        getter = func_02029640(param_1);
        if (getter != 0 && func_0202ff58(getter) == 0) {
            iVar1 = 0;
        }
    }
    if (iVar1 != 0) {
        data_020ce758 = data_020ce758 + (1 << 10);
    }
    if (data_020ce758 >= (1 << 16)) {
        data_020ce758 = 0;
    }
    if (*(unsigned char *)((int)param_1 + 0x44) == 2) {
        param_1[0xf] = 0x1f;
        iVar1 = param_1[0xe] + 2;
        param_1[0xe] = iVar1;
        if (iVar1 > 0x20) {
            *(unsigned char *)(param_1 + 0x11) = 0;
            param_1[0xe] = 0x20;
        }
    }
    else if (*(unsigned char *)((int)param_1 + 0x44) == 3) {
        iVar1 = param_1[0xe] + -2;
        param_1[0xe] = iVar1;
        if (iVar1 < 0) {
            *(unsigned char *)(param_1 + 0x11) = 1;
            param_1[0xe] = 0;
            ((Actor *)param_1)->slot11();
        }
    }
    {
        int iVar2;
        unsigned int uVar3;
        uVar3 = 0;
        iVar1 = 0x20 - ((int)(param_1[0xe] * 0x20 + ((unsigned int)(param_1[0xe] * 0x20 >> 4) >> 0x1b)) >> 5);
        do {
            iVar2 = param_1[uVar3 + 0xc] + iVar1 * (1 - uVar3 * 2);
            param_1[0x10] = iVar1;
            ((Part *)param_1[uVar3 + 10])->slot13(0x8000 - data_020ce758, iVar2 * 0x1000, 0);
            uVar3 = uVar3 + 1;
        } while (uVar3 < 2);
    }
    return;
}
}
