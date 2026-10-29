class Object02016718_0 {
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
    virtual int slot14();
};

class Object02016718_1 {
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
    virtual int slot17();
};

extern "C" {
typedef int code(...);
extern int func_020151f0(...);

unsigned int func_02016718(unsigned char *param_1)

{
    int iVar1;

    iVar1 = func_020151f0(param_1);
    if (iVar1 != 0) {
        return 0;
    }
    if (!(((*(int **)(param_1 + 4) == (int *)0x0) ||
            (iVar1 = ((Object02016718_0 *)(*(int **)(param_1 + 4)))->slot14(), iVar1 != 0)) ||
          (iVar1 = ((Object02016718_1 *)(*(int **)(param_1 + 4)))->slot17(), iVar1 != 0))) {
        return 1;
    }
    return 0;
}
}
