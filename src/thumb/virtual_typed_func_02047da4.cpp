class Object02047da4_0 {
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

class Object02047da4_1 {
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
typedef int code(...);
extern int func_020660c4(...);

void func_02047da4(int param_1)

{
    int iVar1;
    unsigned int uVar2;
    int iVar3;

    uVar2 = 0;
    *(unsigned char *)(param_1 + 0xde) = 0;
    do {
        iVar1 = param_1 + uVar2;
        uVar2 = uVar2 + 1;
        *(unsigned char *)(iVar1 + 0x10c) = 0;
    } while (uVar2 < 6);
    func_020660c4(param_1);
    if ((*(char *)(param_1 + 0xde) == '\0') && (*(char *)(param_1 + 0xdf) != '\0')) {
        ((Object02047da4_0 *)(*(int **)(param_1 + 0xd8)))->slot43();
        *(unsigned char *)(param_1 + 0xdf) = 0;
    }
    uVar2 = 0;
    iVar1 = ((int)0x00000112u) + -6;
    do {
        iVar3 = param_1 + uVar2;
        if ((*(char *)(iVar3 + iVar1) == '\0') && (*(char *)(iVar3 + ((int)0x00000112u)) != '\0')) {
            ((Object02047da4_1 *)(*(int **)(param_1 + uVar2 * 4 + 0xe8)))->slot43();
            *(unsigned char *)(iVar3 + ((int)0x00000112u)) = 0;
        }
        uVar2 = uVar2 + 1;
    } while (uVar2 < 6);
    return;
}
}
