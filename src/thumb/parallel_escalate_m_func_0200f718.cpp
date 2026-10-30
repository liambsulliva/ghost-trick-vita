class Actor {
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
    virtual void *getModel();
};

struct Vec {
    int x;
    int y;
    int z;
};
extern "C" {
extern char data_020cf898[];
int func_02008828(void *object, void *type);
void func_0205958c(Vec *out, void *object, int index);
int func_020083a0(void *object);
void func_02008378(Vec *out, Vec *in, int value);
int func_0201236c(void *object, int value);
void func_0200f61c(unsigned char *object, int x, int y, int scale);

void func_0200f718(unsigned char *object, Actor *actor)
{
    void *model;
    Vec pos;
    Vec base;
    model = actor->getModel();
    if (model == 0) {
        return;
    }
    if (func_02008828(model, data_020cf898) == 0) {
        return;
    }
    func_0205958c(&base, model, 0);
    func_02008378(&pos, &base, func_020083a0(model));
    pos.x = pos.x - 0x80000;
    pos.y = func_0201236c(object + 0xde4, pos.y);
    func_0200f61c(object, pos.x, pos.y, 0x10000);
}
}
