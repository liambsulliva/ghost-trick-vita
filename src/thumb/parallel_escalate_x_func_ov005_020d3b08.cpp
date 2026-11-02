struct Info {
    char pad0[0xc];
    unsigned char ready;
};

class Res {
public:
    virtual void r0();
    virtual void r1();
    virtual void *r2(int size, void *name);
    virtual void *r3(int size);
};

class Icon {
public:
    virtual void i0();
    virtual void i1();
    virtual void i2();
    virtual void i3();
    virtual void i4();
    virtual void i5();
    virtual void i6();
    virtual void i7();
    virtual void i8();
    virtual void i9();
    virtual void i10();
    virtual void i11();
    virtual void i12();
    virtual void i13();
    virtual void i14();
    virtual void i15();
    virtual void i16();
    virtual void i17();
    virtual void i18();
    virtual void i19();
    virtual void i20();
    virtual void i21();
    virtual void i22();
    virtual void i23();
    virtual void i24();
    virtual void i25();
    virtual void i26();
    virtual void i27();
    virtual void i28();
    virtual void i29();
    virtual void i30();
    virtual void i31();
    virtual void i32();
    virtual void i33(int depth);
    virtual void i34();
    virtual void i35();
    virtual void i36();
    virtual void i37();
    virtual void i38();
    virtual void i39();
    virtual void i40();
    virtual void i41();
    virtual void i42();
    virtual void i43();
    virtual void i44();
    virtual void i45();
    virtual void i46();
    virtual void i47();
    virtual void i48();
    virtual void i49();
    virtual void i50();
    virtual void i51();
    virtual void i52();
    virtual void i53();
    virtual void i54();
    virtual void i55();
    virtual void i56();
    virtual void i57();
    virtual void i58();
    virtual void i59();
    virtual void i60();
    virtual void i61();
    virtual void i62();
    virtual void i63();
    virtual void i64();
    virtual void i65(int id);
};

class Menu {
public:
    virtual void m0();
    virtual void m1();
    virtual void m2();
    virtual void m3();
    virtual void m4();
    virtual void m5();
    virtual void m6();
    virtual void m7();
    virtual void m8();
    virtual void m9();
    virtual void m10();
    virtual void m11();
    virtual int m12();
    virtual void m13();
    unsigned char state;
    unsigned char count;
    unsigned char timer;
    char pad07;
    struct Info *info;
    char pad0c[0x14];
    void *items[5];
    void *child;
    void *slot38;
    void *label;
    Icon *icon;
    unsigned char goal;
    unsigned char tone;
    char pad46[2];
    struct Info *current;
    char pad4c[2];
    unsigned char busy;
    unsigned char wait;
    signed char sel;
    unsigned char flags;
};

extern "C" {
extern char data_ov005_020d47f4;
extern char data_ov005_020d47f8;
extern char data_ov005_020d480c;
extern char data_ov005_020d481c;
extern char data_ov005_020d482c;
extern char data_ov005_020d4838;
extern char data_ov005_020d4844;
extern char data_020cfe10;
extern char data_020cefd8;
extern unsigned int data_020c9d50[];
extern void func_ov005_020d44fc(void);
extern void *func_02007bd8(void);
extern Res *func_02007b70(void *a);
extern void func_ov005_020d4098(void *object, int value, int extra);
extern void *func_02009c1c(void);
extern void func_0202f1bc(void *a, void *buf, int b, int c, int d);
extern void *func_0200847c(void);
extern void func_0202a00c(void *a);
extern void func_0202a0c0(void *a, int b);
extern void *func_020312e4(void);
extern void func_02008ef4(void *a, struct Info *info, int b, int c);
extern void func_0203cbe0(void *object, int value);
extern unsigned int func_0203cc14(void *object);
extern void func_0203cc08(void *object, int value);
extern unsigned int func_0203cbfc(void *object);
extern void *func_02008444(void);
extern void func_ov005_020d454c(Menu *p, void *input);
extern void func_02075f84(void);
extern int func_0201572c(void);
extern Icon *func_02065aa4(void *name);
extern void *func_02009900(void);
extern void func_0202f7e8(void *manager, void *object, int a, int b);
extern void *func_02010b7c(void);
extern unsigned int func_02015188(void *object);
extern void func_020521d4(void *object, int a, unsigned int b, int c);
extern void func_0202f64c(void *slot);
extern void func_ov005_020d40a8(int on);
extern void func_ov005_020d443c(Menu *p, unsigned int a, unsigned int b);
extern void func_02067da8(void *part);
extern void func_02067d80(void *part);
extern void func_02067dd0(void *part);
extern void func_0202f134(void *a);
extern void func_0202fad4(void *a, void *data);
extern void func_0200ee34(void *label, int a, int b, int c);
extern void func_ov005_020d40c0(void *child);
extern void func_0200ef08(void *a);
extern void func_0200ef60(void *a);

void func_ov005_020d3b08(Menu *p)
{
    void *buf;
    unsigned int trg;
    int changed;
    signed char i;

    func_ov005_020d44fc();
    switch (p->state) {
    case 0:
        buf = func_02007b70(func_02007bd8())->r3(0x400);
        if (buf != 0) {
            func_ov005_020d4098(buf, 0x2c0, 0x400);
            func_0202f1bc(func_02009c1c(), buf, 0, 0x400, 6);
        }
        buf = func_02007b70(func_02007bd8())->r3(0x400);
        if (buf != 0) {
            func_ov005_020d4098(buf, 0x2c0, 0x400);
            func_0202f1bc(func_02009c1c(), buf, 0, 0x400, 0x14);
        }
        p->state = 1;
        return;
    case 1:
        if ((p->current = p->info) != 0) {
            p->flags |= 2;
            func_0202a00c(func_0200847c());
            p->state = 6;
            return;
        }
        p->current = (struct Info *)func_02007b70(func_02007bd8())->r2(0x1f00, &data_ov005_020d47f4);
        func_02008ef4(func_020312e4(), p->current, 0x1f00, 0);
        func_0203cbe0(func_020312e4(), 1);
        p->state = 2;
        return;
    case 2:
        if (func_0203cc14(func_020312e4()) >= 1) {
            if (func_0203cc14(func_020312e4()) == 1 || func_0203cc14(func_020312e4()) == 4) {
                func_0203cbe0(func_020312e4(), 0);
                p->flags |= 2;
            } else if (func_0203cc14(func_020312e4()) == 2) {
                func_ov005_020d454c(p, &data_ov005_020d47f8);
                p->state = 3;
                p->timer = 0;
                return;
            } else if (func_0203cc14(func_020312e4()) == 3) {
                func_ov005_020d454c(p, &data_ov005_020d480c);
                func_0203cbe0(func_020312e4(), 0);
                p->state = 4;
                return;
            } else if (func_0203cc14(func_020312e4()) == 6) {
                func_ov005_020d454c(p, &data_ov005_020d481c);
                func_0203cbe0(func_020312e4(), 0);
                p->state = 4;
                return;
            } else {
                func_0203cbe0(func_020312e4(), 0);
                p->flags = p->flags & ~2;
            }
            if (p->current != 0 && (((unsigned int)p->flags << 0x1e) >> 0x1f) == 1 && p->current->ready != 0) {
                func_0203cc08(func_02008444(), 1);
            } else {
                func_0203cc08(func_02008444(), 0);
            }
            func_0202a00c(func_0200847c());
            p->state = 6;
            return;
        }
        break;
    case 3:
        p->timer++;
        if (p->timer > 0x10) {
            func_02075f84();
            return;
        }
        break;
    case 4:
        if (func_0201572c() == 0) {
            p->wait++;
            if (p->wait > 0x3c) {
                if (p->icon == 0) {
                    p->icon = func_02065aa4(&data_020cfe10);
                    p->icon->i65(0x36);
                    p->icon->i33(0x10);
                    func_0202f7e8(func_02009900(), p->icon, 1, 0);
                }
                if ((data_020c9d50[1] & 0x1000) != 0 || func_02015188(func_02010b7c()) != 0) {
                    func_020521d4(&data_ov005_020d482c, 0, 0xf, 0x100);
                    func_020521d4(&data_ov005_020d482c, 1, 0xf, 0x100);
                    func_0202a0c0(func_0200847c(), 2);
                    p->state = 5;
                    return;
                }
            }
        }
        break;
    case 5:
        if (func_0201572c() == 0) {
            p->flags = p->flags & ~2;
            func_0202a00c(func_0200847c());
            p->state = 6;
            func_0202f64c(&p->slot38);
            func_0202f64c(&p->icon);
            func_ov005_020d40a8(1);
            return;
        }
        break;
    case 6:
        if ((((unsigned int)p->flags << 0x1e) >> 0x1f) == 1) {
            p->sel = 1;
        }
        func_ov005_020d443c(p, ((unsigned int)p->flags << 0x1e) >> 0x1f, func_0203cbfc(func_02008444()));
        func_020521d4(&data_ov005_020d4838, 0, 0xf, 0x100);
        func_020521d4(&data_ov005_020d4838, 1, 0xf, 0x100);
        p->state = 7;
        return;
    case 7:
        if (func_0201572c() == 0) {
            p->state = 8;
            return;
        }
        break;
    case 8:
        trg = data_020c9d50[3];
        changed = 0;
        if (trg & 0x800) {
            changed = 1;
            func_0202a0c0(func_0200847c(), 1);
            if (((unsigned int)p->flags << 0x1e) >> 0x1f) {
                p->sel++;
                if (p->sel >= 4) {
                    p->sel = 0;
                }
            } else {
                p->sel = 3 - p->sel;
            }
        } else if (trg & 0x400) {
            changed = 1;
            func_0202a0c0(func_0200847c(), 1);
            if (((unsigned int)p->flags << 0x1e) >> 0x1f) {
                p->sel--;
                if (p->sel < 0) {
                    p->sel = 3;
                }
            } else {
                p->sel = 3 - p->sel;
            }
        } else if (trg & 0x300) {
            changed = 1;
            func_0202a0c0(func_0200847c(), 1);
            if (p->sel <= 1) {
                p->sel = 3;
            } else if (((unsigned int)p->flags << 0x1e) >> 0x1f) {
                p->sel = 1;
            } else {
                p->sel = 0;
            }
        }
        if (changed) {
            for (i = 0; i < 4; i++) {
                if (p->items[i] != 0) {
                    if (i == p->sel) {
                        func_02067da8(p->items[i]);
                    } else {
                        func_02067d80(p->items[i]);
                    }
                }
            }
        }
        func_0202f134(func_02010b7c());
        if (p->m12() == 0) {
            break;
        }
        for (i = 0; i < 5; i++) {
            if (p->items[i] != 0) {
                if (i == p->sel) {
                    func_02067dd0(p->items[i]);
                } else {
                    func_02067d80(p->items[i]);
                }
            }
        }
        if (p->sel == 2 || p->sel == 4) {
            func_020521d4(&data_ov005_020d482c, 0, 0x3c, 0x100);
            func_020521d4(&data_ov005_020d482c, 1, 0x3c, 0x100);
            p->state = 0xb;
            p->busy = 1;
            func_0202a0c0(func_0200847c(), 0x1a);
            return;
        }
        func_0202fad4(func_02009900(), &data_020cefd8);
        func_020521d4(&data_ov005_020d4844, 0, p->tone, 0x100);
        func_020521d4(&data_ov005_020d4844, 1, p->tone, 0x100);
        func_0200ee34(p->label, 0x1f, 0, 0);
        p->busy = 1;
        func_0202a0c0(func_0200847c(), 0x1a);
        p->state = 9;
        return;
    case 9:
        func_ov005_020d40c0(p->child);
        func_0200ef08(func_02008444());
        p->state = 0xa;
        return;
    case 10:
        if (p->goal == p->count) {
            func_020521d4(&data_ov005_020d482c, 0, 0x3c, 0x100);
            func_020521d4(&data_ov005_020d482c, 1, 0x3c, 0x100);
            p->state = 0xb;
            return;
        }
        p->count++;
        return;
    case 11:
        if (func_0201572c() == 0) {
            p->m13();
            func_0200ef60(func_02008444());
            p->state = 0xc;
        }
        break;
    default:
        return;
    }
}
}
