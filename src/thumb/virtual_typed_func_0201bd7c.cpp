class Object0201bd7c_0 {
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
};

extern "C" {
extern char data_020a354c;
typedef int code(...);
extern int func_0201bd0c(...);
extern int func_0201bd78(...);
extern int func_0201bdd4(...);
extern int func_0202f64c(...);
extern int func_02031270(...);

unsigned int * func_0201bd7c(unsigned int *param_1)

{
    int *piVar1;
    unsigned int *puVar2;
    unsigned int uVar3;
    unsigned int uVar4;

    *param_1 = ((unsigned int)&data_020a354c);
    for (uVar4 = 0; uVar3 = func_0201bd78(param_1 + 1), uVar4 < uVar3; uVar4 = uVar4 + 1) {
        piVar1 = (int *)func_0201bdd4(param_1 + 1,uVar4);
        if (*piVar1 != 0) {
            puVar2 = (unsigned int *)func_0201bdd4(param_1 + 1,uVar4);
            ((Object0201bd7c_0 *)(*puVar2))->slot43();
        }
        func_0201bdd4(param_1 + 1,uVar4);
        func_0202f64c();
    }
    func_0201bd0c(param_1 + 1);
    func_02031270(param_1);
    return param_1;
}
}
