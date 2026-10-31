class Obj020453fc {
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
    virtual void slot60();
    virtual void slot61();
    virtual void slot62();
    virtual void slot63();
    virtual void slot64();
    virtual void slot65();
    virtual void slot66(void *pos);
    virtual void slot67();
    virtual void slot68(void *self,void *name,int kind);
};

extern "C" {
extern char data_0209b534;
extern char data_020cdcf0;
extern void *func_02008444(void);
extern int func_0200f320(void *system);
extern int func_020454e4(int value);
extern void func_02027458(unsigned int value);
extern void func_02010384(void *system,int value);
extern void func_0200f718(void *system,void *actor);
extern void *func_02011264(void);
extern char *func_0201123c(void *base,int index);
extern char *func_02022fc0(char *base,char *name);
extern void func_020454ec(void *object,int value);
extern void func_020110b8(void *system);

int func_020453fc(unsigned char *p)
{
    char *found;

    if (p[0x119] != 0) {
        return 0;
    }
    if (((unsigned int)p[0xe0] << 29) >> 31 != 0) {
        return 0;
    }
    if (func_0200f320(func_02008444()) != 0) {
        return 0;
    }
    if (func_020454e4(*(int *)(p + 0x28)) > 0x90000) {
        return 0;
    }
    if (*(int *)(p + 0x2c) > 0x60000) {
        return 0;
    }
    if (*(int *)(p + 0x2c) < (int)0xfffa0000) {
        return 0;
    }
    if (p[0x11a] == 0) {
        func_02027458(*(unsigned int *)(p + 0x120));
    }
    ((Obj020453fc *)p)->slot66(p + 0x28);
    ((Obj020453fc *)p)->slot68(p,&data_0209b534,5);
    p[0x119] = 1;
    func_02010384(func_02008444(),1);
    if (p[0x11c] == 0) {
        func_0200f718(func_02008444(),p);
    }
    found = func_02022fc0(func_0201123c(func_02011264(),0),&data_020cdcf0);
    if (found != 0) {
        func_020454ec(found,1);
        func_020110b8(func_02008444());
    }
    return 1;
}
}
