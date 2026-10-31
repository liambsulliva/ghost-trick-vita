class Obj
{
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
    virtual void slot12(int *v);
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
    virtual void slot33(int value);
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
    virtual void slot53(int value);
    virtual void slot54();
    virtual void slot55();
    virtual void slot56();
    virtual void slot57();
    virtual void slot58();
    virtual void slot59();
    virtual void slot60(int value);
    virtual void slot61();
    virtual void slot62();
    virtual void slot63();
    virtual void slot64();
    virtual void slot65(int value);
    virtual void slot66();
    virtual void slot67();
    virtual void slot68();
    virtual void slot69();
    virtual void slot70();
    virtual void slot71();
    virtual void slot72();
    virtual void slot73();
    virtual void slot74();
    virtual void slot75();
    virtual void slot76();
    virtual void slot77();
    virtual void slot78();
    virtual void slot79();
    virtual void slot80();
    virtual void slot81();
    virtual void slot82();
    virtual void slot83();
    virtual void slot84();
    virtual void slot85();
    virtual void slot86(int value);
};

extern "C" {
extern char data_020cfe80[];
extern Obj *func_02065b30(void *data);
extern void func_020153d8(Obj *obj, int owner);
extern void func_020082d8(int *v, int src);
extern void *func_02008444(void);
extern int func_0200f5c8(void *a);
extern int func_0200f5e8(void *a);
extern int func_02031b44(void *a);
extern void *func_0200847c(void);
extern void func_0202a0c0(void *a, int b);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, Obj *obj, int b, int c);

void func_02045588(int p, int src)
{
    int v[3];
    Obj *obj;
    int res;

    obj = func_02065b30(data_020cfe80);
    *(Obj **)(p + 0x128) = obj;
    if (obj != 0) {
        func_020082d8(v, src);
        v[0] = v[0] + func_0200f5c8(func_02008444()) + 0x80000;
        v[1] = -v[1] + func_0200f5e8(func_02008444()) + 0x60000;
        (*(Obj **)(p + 0x128))->slot65(3);
        (*(Obj **)(p + 0x128))->slot86(1);
        (*(Obj **)(p + 0x128))->slot33(0x420);
        (*(Obj **)(p + 0x128))->slot12(v);
        func_020153d8(*(Obj **)(p + 0x128), p);
        (*(Obj **)(p + 0x128))->slot53(0);
        res = func_02031b44(func_02008444());
        *(int *)(p + 0x12c) = res;
        (*(Obj **)(p + 0x128))->slot60(res);
        func_0202a0c0(func_0200847c(), 0x11);
        func_0202f7e8(func_02009900(), *(Obj **)(p + 0x128), 8, 0);
    }
}
}
