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

struct Owner {
    unsigned char pad[0xdfc];
    Effect *slots[8];
    int values[8];
};
extern "C" {
extern char data_020ce6b4[];
unsigned int func_02010820(Effect *effect);
void func_0202f64c(Effect **slot);
Effect *func_0204449c(char *heap);
void *func_02009900(void);
void func_0202f7e8(void *manager, Effect *effect, int kind, int flag);

void func_02010778(Owner *owner, int a, int b, int c, short value)
{
    unsigned int i;
    unsigned int j;
    Effect *effect;
    for (i = 2; i < 8; i++) {
        if (owner->slots[i] == 0) {
            break;
        }
    }
    if (i == 8) {
        i = 2;
        for (j = 2; j < 8; j++) {
            if (func_02010820(owner->slots[j]) < func_02010820(owner->slots[i])) {
                i = j;
            }
        }
        func_0202f64c(&owner->slots[i]);
    }
    effect = func_0204449c(data_020ce6b4);
    if (effect != 0) {
        effect->setup(a, c, b);
        effect->update();
        func_0202f7e8(func_02009900(), effect, 7, 0);
        owner->slots[i] = effect;
        owner->values[i] = value;
    }
}
}
