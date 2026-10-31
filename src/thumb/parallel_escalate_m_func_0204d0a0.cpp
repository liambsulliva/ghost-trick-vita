class Part0204d0a0 {
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
    virtual void slot35(int value);
};

extern "C" {
struct Self0204d0a0 {
    char pad[0xb8];
    Part0204d0a0 *part;
    unsigned short target;
    unsigned short current;
    int zoom;
};
struct Field0204d0a0;
extern char data_020ceb5c;
extern void *func_02008444(void);
extern int func_0200f330(void *system);
extern void *func_02009900(void);
extern struct Field0204d0a0 *func_0202fb48(void *system,void *kind,int start);
extern void func_0204d18c(struct Field0204d0a0 *object,int value);

void func_0204d0a0(struct Self0204d0a0 *self)
{
    struct Field0204d0a0 *field;

    if (func_0200f330(func_02008444()) != 0) {
        return;
    }
    if (self->target < self->current) {
        self->zoom += 0x340;
        field = func_0202fb48(func_02009900(),&data_020ceb5c,-1);
        if (field != 0) {
            func_0204d18c(field,self->zoom);
        }
        if (self->zoom > 0x19000) {
            self->zoom = 0x19000;
            self->current = self->target;
        }
    }
    if (self->target > self->current) {
        self->zoom -= 0x340;
        field = func_0202fb48(func_02009900(),&data_020ceb5c,-1);
        if (field != 0) {
            func_0204d18c(field,self->zoom);
        }
        if (self->zoom < 0xc000) {
            self->zoom = 0xc000;
            self->current = self->target;
        }
    }
    self->part->slot35((signed char)(self->zoom >> 12));
}
}
