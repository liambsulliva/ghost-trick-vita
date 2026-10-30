class Effect {
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
    virtual void update();
    virtual void slot54();
    virtual void slot55();
    virtual void slot56();
    virtual void slot57();
    virtual void slot58();
    virtual void slot59();
    virtual void slot60();
    virtual void setup(int a, int b, int c);
};

extern "C" {
void func_02010754(char *object);
void func_020106a0(char *object);
void func_02010428(void *object, int value);
void *func_02009900(void);
void func_0202f7e8(void *manager, void *object, int kind, int flag);
int func_02010420(void *object);
void func_02045930(void *object, int a, int b, int c);
void func_02010524(void *object, int value);
void func_0204649c(void *object, int a, int b, int c);
void func_020105b0(void *object, unsigned char value);
void func_02010680(void *object, int value);
}

extern "C" {
extern char data_020ce720[];
Effect *func_02044e90(char *heap);

void func_02010440(char *object, int a, int b, int c, int d, int e, int f)
{
    Effect *effect;
    func_02010754(object);
    if (b == 0) {
        return;
    }
    if (((*(unsigned int *)(object + 8) << 8) >> 24) == 2) {
        return;
    }
    effect = func_02044e90(data_020ce720);
    if (effect == 0) {
        return;
    }
    effect->setup(a, 1, 0);
    func_02045930(effect, c, d, e);
    func_02010428(effect, f);
    effect->update();
    func_0202f7e8(func_02009900(), effect, 8, 0);
    *(Effect **)(object + 0xdfc) = effect;
    *(int *)(object + 0xe1c) = b;
}
}
