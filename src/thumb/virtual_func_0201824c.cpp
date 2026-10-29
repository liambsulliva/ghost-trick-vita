class Object0201824c_0 {
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
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual void slot21();
    virtual void slot22();
    virtual void slot23();
    virtual void slot24();
    virtual int slot25();
};

class Object0201824c_1 {
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
    virtual int slot15();
};

extern "C" {
typedef int code(...);
extern int func_0200847c(...);
extern int func_0202a0c0(...);

unsigned int func_0201824c(int *param_1)

{
    int iVar1;
    unsigned int uVar2;

    iVar1 = ((Object0201824c_0 *)param_1)->slot25();
    if (iVar1 == 0) {
        if (((*((unsigned int *)0x020c9d50u) & 0x200000) == 0) || (iVar1 = ((Object0201824c_1 *)param_1)->slot15(), iVar1 == 0)
              ) {
            uVar2 = func_0200847c();
            func_0202a0c0(uVar2,*(unsigned short *)(param_1[5] + 2));
        }
    }
    return 0;
}
}
