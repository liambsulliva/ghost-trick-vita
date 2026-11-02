class Label {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
};

struct Owner {
    char pad0[8];
    unsigned int flags8_low : 7;
    unsigned int dirty : 1;
    unsigned int flags8_mid : 7;
    unsigned int locked : 1;
    unsigned int flags8_high : 16;
    unsigned int flagsc_0 : 1;
    unsigned int moving : 1;
    unsigned int flagsc_2 : 8;
    unsigned int scroll : 1;
    unsigned int flagsc_11 : 2;
    unsigned int fade : 2;
    unsigned int flagsc_15 : 8;
    unsigned int shown : 1;
    unsigned int flagsc_24 : 8;
    unsigned int flags10_low : 26;
    unsigned int busy : 1;
    unsigned int flags10_high : 5;
    char pad14[0xdc4];
    int list_dd8[3];
    char menu_de4[0x58];
    int timer;
    char pad_e40[0xac];
    Label *top;
    Label *bottom;
    char pad_ef4[0x20];
    int count;
    int total;
    char pad_f1c[0x98];
    Label *icon;
};

extern "C" {
extern char data_020cedc4[];
extern char data_020ce7d4[];
extern char data_020cfe2c[];
extern char data_020ce900[];
extern char data_020ca128[];
extern char data_020cfb0c[];
extern char data_020cf8ec[];
extern char data_020cf8d0[];
extern char data_020cdd58[];
extern char data_020cdd90[];
extern char data_020cf75c[];
extern char data_020cfb90[];
extern char data_020a46c8[];
extern void *func_02009900(void);
extern void func_0202fad4(void *a, char *b);
extern void func_0201017c(Owner *owner);
extern int func_02027518(int id);
extern void func_02027538(int id);
extern void func_02010264(Owner *owner, int value);
extern int func_0202f64c(Label **label);
extern void func_02010824(Owner *owner, int value);
extern void func_02011958(Owner *owner, int a, int b);
extern void *func_0201003c(void);
extern void func_020159a0(void *object);
extern void func_02011d74(Owner *owner);
extern Label *func_0202fb48(void *a, char *kind, int start);
extern void func_0201001c(Label *label);
extern void *func_0200b208(void);
extern void func_02023ef4(void *object);
extern void func_02012128(void *menu);
extern void func_020083b0(int *dst, char *src);
extern void func_0200fbc8(Owner *owner);
extern void func_02010028(Owner *owner, int value);
void func_0200fe74(Owner *owner);
}

void func_0200fe74(Owner *owner)
{
    Label *label;

    owner->count = 0;
    owner->total = 0;
    owner->moving = 0;
    owner->shown = 0;
    owner->busy = 0;
    owner->fade = 0;
    owner->timer = 0;
    func_0201017c(owner);
    func_0202fad4(func_02009900(), data_020cedc4);
    if (func_02027518(0x1008) == 0) {
        func_0202fad4(func_02009900(), data_020ce7d4);
    }
    func_02027538(0x1008);
    func_02010264(owner, -1);
    func_0202fad4(func_02009900(), data_020cfe2c);
    func_0202fad4(func_02009900(), data_020ce900);
    func_0202fad4(func_02009900(), data_020ca128);
    func_0202fad4(func_02009900(), data_020cfb0c);
    func_0202fad4(func_02009900(), data_020cf8ec);
    func_0202fad4(func_02009900(), data_020cf8d0);
    func_0202fad4(func_02009900(), data_020cdd58);
    func_0202fad4(func_02009900(), data_020cdd90);
    func_0202fad4(func_02009900(), data_020cf75c);
    func_0202f64c(&owner->top);
    func_0202f64c(&owner->bottom);
    func_02010824(owner, 1);
    func_02011958(owner, 0, 0);
    func_020159a0(func_0201003c());
    func_02011d74(owner);
    label = func_0202fb48(func_02009900(), data_020cfb90, -1);
    if (label != 0) {
        func_0201001c(label);
        label->s11();
    }
    func_0202f64c(&owner->icon);
    func_02023ef4(func_0200b208());
    func_02012128(owner->menu_de4);
    func_020083b0(owner->list_dd8, data_020a46c8);
    owner->scroll = 0;
    owner->locked = 0;
    func_0200fbc8(owner);
    func_02010028(owner, 0);
}
