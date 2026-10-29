class Object02018288_0 {
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

extern "C" {
typedef int code(...);
extern unsigned char * func_0200847c(...);
extern int func_0202a16c(...);
extern int func_0202a1b0(...);

unsigned int func_02018288(int *param_1)

{
    unsigned short uVar1;
    int iVar2;
    unsigned char *puVar3;

    iVar2 = ((Object02018288_0 *)(param_1))->slot25();
    if (iVar2 == 0) {
        uVar1 = *(unsigned short *)(param_1[5] + 2);
        if (uVar1 == 0) {
            puVar3 = func_0200847c();
            func_0202a16c((int)puVar3,(unsigned char *)0x0);
        }
        else {
            puVar3 = func_0200847c();
            func_0202a1b0((int)puVar3,(unsigned int)uVar1,0);
        }
    }
    return 0;
}
}
