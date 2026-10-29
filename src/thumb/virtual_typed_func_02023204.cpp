class Object02023204_0 {
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
};

class Object02023204_1 {
public:
    virtual void slot0();
    virtual void slot1();
};

extern "C" {
typedef int code(...);
extern int func_0206cdd0(...);

void func_02023204(unsigned int param_1,int *param_2)

{
    int iVar1;

    ((Object02023204_0 *)(*param_2))->slot8();
    iVar1 = *param_2;
    if ((*(char *)(iVar1 + 0xf) != '\0') && (*(char *)(iVar1 + 0x10) != '\0')) {
        func_0206cdd0(0,*(unsigned int *)(iVar1 + 0x18));
    }
    if ((int *)*param_2 != (int *)0x0) {
        ((Object02023204_1 *)(*param_2))->slot1();
    }
    *param_2 = 0;
    return;
}
}
