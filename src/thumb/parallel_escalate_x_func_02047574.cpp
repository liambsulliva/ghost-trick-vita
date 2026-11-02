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
    virtual void slot35(signed char);
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
    virtual void slot56(short);
};

struct Scene {
    unsigned char pad0[0x40];
    unsigned short delay;
    unsigned char pad42[0x76];
    int handle;
    Object *speaker;
    unsigned short timer;
    unsigned char padc2[0x2];
    int line;
    unsigned char busy;
    unsigned char pending;
    unsigned char padca[0x2];
    int level;
    int speed;
    Object *meter;
};

extern "C" {
extern int func_0202ff58(Scene *p);
extern void func_02010dc4(Object *object);
extern void func_02008418(Object *object);
extern void *func_02008444(void);
extern int func_02031af4(void *manager, int handle);
extern void func_02031b04(void *manager, int *handle);
extern void func_0200da04(char *a, char *b, char *c, char *d);
extern void func_02010824(void *manager, int value);
extern void func_02010f20(void *manager, int value);
extern int func_020662c4(Object *object, int value);
extern Object *func_02063b70(void *heap);
extern void func_020153d8(Object *object, Scene *scene);
extern void func_0203575c(Object *object, Scene *scene);
extern void *func_02009900(void);
extern void func_0202f7e8(void *system, Object *object, int a, int b);
extern void func_02010da4(void *manager, int value);
extern char data_0209b7f4[];
extern char data_0209b7f8[];
extern char data_0209b800[];
extern char data_0209b810[];
extern char data_0209b818[];
extern char data_020cfd44[];
void func_02047574(Scene *p);
}

void func_02047574(Scene *p)
{
    int result;

    if (func_0202ff58(p) != 0) {
        if (p->speaker != 0) {
            func_02010dc4(p->speaker);
        }
    } else {
        if (p->speaker != 0) {
            func_02008418(p->speaker);
        }
        return;
    }

    if (p->speaker != 0) {
        if (func_02031af4(func_02008444(), p->handle) != 0) {
            p->speaker = 0;
            func_02031b04(func_02008444(), &p->handle);
            func_0200da04(data_0209b7f4, data_0209b7f8, data_0209b800, data_0209b810);
            func_02010824(func_02008444(), 0);
            func_02010f20(func_02008444(), 0);
        } else {
            result = func_020662c4(p->speaker, 0);
            if (result == 1) {
                func_0200da04(data_0209b7f4, data_0209b7f8, data_0209b818, data_0209b810);
            } else if (result == 2) {
                p->pending = 1;
            }
            if (p->pending != 0 && p->busy == 0) {
                p->pending = 0;
                p->speaker->slot56(p->line);
            }
            return;
        }
    }

    if (p->meter == 0) {
        if (p->timer++ >= p->delay) {
            p->meter = func_02063b70(data_020cfd44);
            p->meter->slot35(0);
            func_020153d8(p->meter, p);
            func_0203575c(p->meter, p);
            func_0202f7e8(func_02009900(), p->meter, 1, 0);
            func_02010da4(func_02008444(), 1);
        }
    }

    if (p->meter != 0) {
        if ((p->level += p->speed) > 0x1f000) {
            p->level = 0x1f000;
        }
        p->meter->slot35(p->level >> 12);
    }
}
