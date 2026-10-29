class Object0202702c_0 {
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
    virtual void slot25();
    virtual void slot26();
    virtual void slot27();
    virtual void slot28();
    virtual void slot29(int argument0);
};

extern "C" {
typedef int code(...);
extern int func_02008828(...);

void func_0202702c(unsigned int param_1,int *param_2,int param_3)

{
    int iVar1;

    if ((param_2 != (int *)0x0) &&
          (iVar1 = func_02008828((unsigned char *)param_2,((unsigned char *)0x020cf898u)), iVar1 != 0)) {
        ((Object0202702c_0 *)(param_2))->slot29((int)((unsigned int)*(unsigned short *)(param_3 + 4) << 4));
    }
    return;
}
}
