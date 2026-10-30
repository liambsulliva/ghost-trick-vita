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
    virtual void slot45(int value);
};

struct Flags {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int bit2 : 1;
    unsigned int bit3 : 1;
    unsigned int bit4 : 1;
};

struct Track {
    void *vtable;
    char pad4[0x48];
    int handle;
    char pad50[0xc];
    unsigned short id;
    unsigned char byte5e;
    unsigned char byte5f;
    char pad60[8];
    int value68;
    char pad6c[8];
    int value74;
    Flags flags;
};

extern "C" {
extern int func_02042bb0(unsigned short id, unsigned char *slot, int a, unsigned char b, int c, int d, int e, int f);
}

extern "C" void func_02057488(Track *self, unsigned short id, int a, unsigned char b, int c, int d, int e, int f)
{
    if (self->id != id) {
        ((Object *)self)->slot43();
        self->id = id;
        self->byte5e = 0xff;
        self->byte5f = b;
        self->flags.bit3 = c;
        self->flags.bit4 = d;
        self->value68 = e;
        self->flags.bit1 = 0;
        self->value74 = f;
        self->handle = func_02042bb0(self->id, &self->byte5e, a, self->byte5f, self->flags.bit3, self->flags.bit4, self->value68, self->value74);
        ((Object *)self)->slot45(self->handle);
    }
}
