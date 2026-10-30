class Object {
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
    virtual void slot53();
    virtual void slot54();
    virtual void slot55();
    virtual void slot56();
    virtual void slot57();
    virtual void slot58();
    virtual void slot59();
    virtual void slot60(int value);
};

class Factory {
public:
    virtual Object *slot0();
};

extern "C" {
extern void *func_02008444(void);
extern int func_02031af4(void *system, int id);
extern void func_02031b04(void *system, int *id);
extern int func_02031b44(void *system);
extern int func_02010b54(void *self);
extern int func_020266f4(void *self);
extern void *func_020266dc(void *self, int index);
extern int func_02036f3c(void *value);
extern void func_020153d8(Object *kid, void *owner);
extern void *func_02009900(void);
extern void func_0202f7e8(void *system, Object *object, int layer, int flags);
}

extern "C" void func_020538a4(char *self, Factory *factory)
{
    Object *child;
    if (*(unsigned char *)(self + 0x10) == 0) {
        if (func_02031af4(func_02008444(), *(int *)(self + 0x158)) != 0) {
            func_02031b04(func_02008444(), (int *)(self + 0x158));
            *(unsigned char *)(self + 0x10) = 1;
            return;
        }
        if (func_02010b54(self) != 0 && *(unsigned char *)(self + 0x154) == 0 && func_020266f4(self) != 0) {
            if (func_02036f3c(func_020266dc(self, 0)) == 0) {
                child = factory->slot0();
                if (child != 0) {
                    func_020153d8(child, self);
                    *(int *)(self + 0x158) = func_02031b44(func_02008444());
                    child->slot60(*(int *)(self + 0x158));
                    func_0202f7e8(func_02009900(), child, 7, 0);
                    *(unsigned char *)(self + 0x154) = 1;
                }
            }
        }
    }
}
