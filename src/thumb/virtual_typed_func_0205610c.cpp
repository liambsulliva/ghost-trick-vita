class Object0205610c_0 {
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
    virtual void slot29();
    virtual void slot30();
    virtual void slot31();
    virtual void slot32();
    virtual void slot33();
    virtual void slot34();
    virtual void slot35();
    virtual void slot36();
    virtual void slot37();
    virtual void slot38();
    virtual void slot39();
    virtual void slot40();
    virtual void slot41();
    virtual void slot42();
    virtual void slot43();
    virtual void slot44();
    virtual void slot45();
    virtual void slot46();
    virtual void slot47();
    virtual void slot48();
    virtual void slot49();
    virtual void slot50();
    virtual void slot51();
    virtual void slot52();
    virtual int slot53();
};

extern "C" {
extern char data_020c9d50;
typedef int code(...);
extern int func_0200847c(...);
extern int func_0202a0c0(...);
extern int func_0202a16c(...);
extern int func_0203bfb4(...);
extern int func_02056164(...);

void func_0205610c(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    if (((*(int *)(param_1 + 0x2c) != 0) && (iVar1 = func_02056164(), iVar1 != 0)) &&
          (*(int *)(param_1 + 0x28) == 0)) {
        iVar1 = ((Object0205610c_0 *)(*(int **)(param_1 + 0x2c)))->slot53();
        if ((iVar1 == 0) && ((*(unsigned int *)(((int)&data_020c9d50) + 4) & 0x1000) == 0)) {
            return;
        }
        uVar2 = func_0200847c();
        func_0202a16c(uVar2,0);
        uVar2 = func_0200847c();
        func_0202a0c0(uVar2,2);
        *(unsigned int *)(param_1 + 0x28) = 1;
        func_0203bfb4(*(unsigned int *)(param_1 + 0x2c),2);
    }
    return;
}
}
