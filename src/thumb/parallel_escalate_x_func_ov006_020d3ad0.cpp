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
    virtual void s12();
    virtual void s13(int x, int y, int z);
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33(int depth);
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42(int id, int a, int b, int c, int d, int e, int f);
};

class Item {
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
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32(void *record, int index, int mode);
};

class Model {
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
    virtual void s12();
    virtual void s13(int x, int y, int z);
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59();
    virtual void s60();
    virtual void s61(int id);
};

class Part {
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
    virtual void s12();
    virtual void s13(int x, int y, int z);
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33(int depth);
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53(int on);
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59();
    virtual void s60();
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
    virtual void s65(int id);
};

class Gauge {
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
    virtual void s12();
    virtual void s13(int x, int y, int z);
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53(int value, int id);
};

struct Record {
    unsigned char pad00[8];
    signed char link;
    signed char slot;
    unsigned char kind;
    unsigned char pad0b[0x29];
};

struct Entry {
    unsigned short id;
    unsigned short pad02;
    unsigned short count;
    unsigned short pad06;
    int pad08;
    struct Record *records;
};

struct Panel {
    unsigned char pad00[0x20];
    int mode;
    unsigned char b24;
    signed char index;
    unsigned char b26;
    unsigned char state;
    unsigned char step;
    unsigned char page;
    unsigned char pad2a[0x44 - 0x2a];
    int f44;
    int f48;
    Label *label;
    Gauge *f50;
    Gauge *f54;
    unsigned char pad58[0x64 - 0x58];
    void *f64;
    Model *f68;
    Model *f6c;
    Item *items[9];
    int f94;
};

extern "C" {
extern struct Entry data_ov006_020dacdc[];
extern char data_020cf724;
extern char data_020cd904;
extern char data_020ce970;
extern char data_020cfe10;
extern char data_020ce98c;
extern char data_020ce2b4;
extern char data_020cd8e8;
extern void *func_02008444(void);
extern int func_ov006_020d3fc8(int object);
extern void func_0201134c(void *a);
extern void func_02011340(void *a);
extern void *func_0201ff10(void *a, int b);
extern int func_0201f944(void *object, int page);
extern void func_ov006_020d4098(struct Panel *p, unsigned char a, signed char b, unsigned char c, unsigned int d);
extern unsigned int func_ov006_020d3fd4(void *a);
extern Label *func_0205693c(void *type);
extern void *func_02009900(void);
extern void func_0202f7e8(void *manager, void *object, int a, int b);
extern void *func_02009c1c(void);
extern void func_0202f3a4(void *a, int b, int c);
extern void func_0202f64c(void *slot);
extern Item *func_02033d78(void *type);
extern void func_ov006_020d3fe0(Item *item, unsigned char index, unsigned char kind, Item **items);
extern void func_020593f8(Item *item, Item *parent, int link, int slot);
extern Model *func_02046fb8(void *type);
extern Model *func_02046fcc(void *type);
extern Model *func_02065aa4(void *type);
extern Part *func_020082f8(Model *model, void *type);
extern void func_0203575c(Model *model, struct Panel *p);
extern void func_02052c28(Model *model, Item *item, int a, int b);
extern void *func_0203fb48(void *type);
extern void func_0203fcf8(void *object, int a, int b, int c, int d);
extern Gauge *func_02033db4(void *type);
extern void func_ov006_020d42d8(struct Panel *p, unsigned char a, signed char b);

void func_ov006_020d3ad0(struct Panel *p)
{
    int n;
    int result;
    void *object;
    unsigned int base;
    Item **items;
    unsigned char i;
    Part *part;

    if (p->f94 != 0) {
        switch (p->step) {
        case 0:
            break;
        case 1:
            if (p->f94 == 0 || (p->f94 != 0 && func_ov006_020d3fc8(p->f94) != 0)) {
                func_0201134c(func_02008444());
                p->step = 2;
            }
            break;
        case 2:
            break;
        case 3:
            if (p->f94 != 0 && func_ov006_020d3fc8(p->f94) != 0) {
                func_02011340(func_02008444());
                p->step = 0;
            }
            break;
        }
    }
    if (p->state == 0) {
        return;
    }
    if (p->state == 1) {
        n = 10;
        if (p->mode == 0) {
            object = func_0201ff10(func_02008444(), 1);
            if (object != 0) {
                result = func_0201f944(object, p->page + 1);
                if (result >= 0) {
                    n = result;
                }
            }
        }
        func_ov006_020d4098(p, 0, p->page, 0, n);
    } else if (p->state == 2) {
        if (p->b24 == 0) {
            base = func_ov006_020d3fd4(func_02008444());
            if (p->label == 0) {
                p->label = func_0205693c(&data_020cf724);
                p->label->s13(0x40000, 0, 0);
                p->label->s33(0x2d0);
                func_0202f7e8(func_02009900(), p->label, 1, 0);
            }
            if (p->mode == 1 && p->index == 0x2c) {
                p->label->s42(0x12c, 0, 1, 0, 1, -1, base + 0x64000);
            } else {
                p->label->s42(data_ov006_020dacdc[p->index].id, 0, 1, 0, 1, -1, base + 0x64000);
            }
            if (p->mode == 0) {
                if (p->index == 0x2c) {
                    func_0202f3a4(func_02009c1c(), 0x10, 1);
                } else {
                    func_0202f3a4(func_02009c1c(), 0x10, 0);
                }
                func_0202f3a4(func_02009c1c(), 4, 0);
            }
            i = 0;
            items = p->items;
            for (; i < 9; i++) {
                func_0202f64c(&items[i]);
                if (i < data_ov006_020dacdc[p->index].count) {
                    p->items[i] = func_02033d78(&data_020cd904);
                    func_0202f7e8(func_02009900(), p->items[i], 7, 0);
                    p->items[i]->s32(&data_ov006_020dacdc[p->index].records[i], p->index, p->mode);
                    func_ov006_020d3fe0(p->items[i], i, data_ov006_020dacdc[p->index].records[i].kind, items);
                    if (i != 0 && data_ov006_020dacdc[p->index].records[i].link != -1) {
                        func_020593f8(p->items[i], p->items[0], data_ov006_020dacdc[p->index].records[i].link, data_ov006_020dacdc[p->index].records[i].slot);
                    } else {
                        func_020593f8(p->items[i], 0, -1, -1);
                    }
                }
            }
            if (p->index == 0) {
                p->f68 = func_02046fb8(&data_020ce970);
                p->f68->s61(0x8002);
                func_0203575c(p->f68, p);
                func_0202f7e8(func_02009900(), p->f68, 7, 0);
            }
            if (p->index == 0xd) {
                p->f68 = func_02065aa4(&data_020cfe10);
                part = func_020082f8(p->f68, &data_020cfe10);
                part->s65(0x97);
                part->s53(0);
                part->s33(0x800);
                func_02052c28(p->f68, p->items[0], 1, -1);
                func_0203575c(p->f68, p);
                func_0202f7e8(func_02009900(), p->f68, 7, 0);
            }
            if (p->index == 0x21) {
                p->f68 = func_02065aa4(&data_020cfe10);
                part = func_020082f8(p->f68, &data_020cfe10);
                part->s65(0x6b);
                part->s53(0);
                part->s33(0x800);
                part->s13(0x36000, 0x23000, 0);
                func_0203575c(p->f68, p);
                func_0202f7e8(func_02009900(), p->f68, 7, 0);
            }
            if (p->index == 0x28) {
                p->f68 = func_02046fcc(&data_020ce98c);
                p->f68->s61(0x8003);
                p->f68->s13(0x65000, 0x63000, 0);
                func_020082f8(p->f68, &data_020ce98c)->s65(1);
                func_0203575c(p->f68, p);
                func_0202f7e8(func_02009900(), p->f68, 7, 0);
                p->f6c = func_02065aa4(&data_020cfe10);
                part = func_020082f8(p->f6c, &data_020cfe10);
                part->s65(0x70);
                part->s53(0);
                part->s33(0x800);
                part->s13(0x65000, 0x90000, 0);
                func_0203575c(p->f6c, p);
                func_0202f7e8(func_02009900(), p->f6c, 7, 0);
            }
        } else {
            if (p->b26 != 0 && p->mode == 0 && p->f64 == 0) {
                p->f64 = func_0203fb48(&data_020ce2b4);
                func_0203fcf8(p->f64, 0x1b4d, 4, 0, 2);
                func_0202f7e8(func_02009900(), p->f64, 1, 0);
            }
            if (p->f54 == 0) {
                p->f54 = func_02033db4(&data_020cd8e8);
                p->f54->s13(0, 0x54000, 0);
                func_0202f7e8(func_02009900(), p->f54, 1, 0);
            }
            p->f54->s53(p->f48, 0x4f);
        }
        if (p->f50 == 0) {
            p->f50 = func_02033db4(&data_020cd8e8);
            p->f50->s13(0, -0x54000, 0);
            func_0202f7e8(func_02009900(), p->f50, 1, 0);
        }
        if (p->f50 != 0) {
            p->f50->s53(p->f44, 0x4e);
        }
        p->state = 3;
    } else if (p->state == 3) {
        func_ov006_020d42d8(p, p->b24, p->index);
        p->state = 4;
    }
}
}
