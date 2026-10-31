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
    virtual void slot35(signed char value);
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
};
extern "C" {
extern const char data_0209ab10[];
extern const char data_0209ab20[];
extern void *func_020312e4(void);
extern unsigned int func_0203cc14(void *object);
extern void func_0203cbe0(void *object, int value);
extern void func_0203cc08(void *object, int value);
extern void *func_02008444(void);
extern void *func_02009900(void);
extern void func_02008fb0(void *object);
extern void func_0200fbc8(void *object);
extern int func_02027d4c(const char *name, int kind);
extern void *func_0202f624(int size);
extern Object *func_02067a84(void *object);
extern void func_0202f7e8(void *list, Object *object, int a, int b);
extern int func_0203ce68(unsigned char *p, int open);
extern void func_0203d028(unsigned char *p, int open, unsigned int id, unsigned int scale, int flag);
extern int func_0203d060(unsigned char *p, int open, int a, unsigned int b, int flag);
extern void func_0203cf74(unsigned char *p, int open, int flag);
extern void func_020521d4(const char *name, int a, int b, int c);
extern void func_02075f84(void);
int func_0203d670(unsigned char *p)
{
    unsigned char state;
    int open;
    int r;
    Object *o;
    void *m;
    state = p[4];
    if (state <= 1) {
        open = 0;
        if (state == 1) {
            o = *(Object **)(p + 0x38);
            open = 1;
            if (o != 0) {
                o->slot35(p[0x60] >> 1);
            }
            o = *(Object **)(p + 0x3c);
            if (o != 0) {
                o->slot35(p[0x60] >> 1);
            }
            p[0x60]++;
            if (p[0x60] < 0x3e) {
                return 1;
            }
            p[0x60] = 0x3e;
        }
        r = func_0203ce68(p, open);
        if (r == -2) {
            return 1;
        }
        if (p[4] == 0) {
            if (r == 0) {
                p[4] = 6;
                func_0203d028(p, 0, 0x156, 0x14000, 1);
            } else if (r == 1) {
                func_020521d4(data_0209ab10, 0, 0x3c, 0x100);
                p[4] = 2;
            }
        } else {
            if (r == 0) {
                func_02008fb0(func_020312e4());
                func_0203cbe0(func_020312e4(), 1);
                p[4] = 5;
            } else if (r == 1) {
                p[4] = 7;
                func_0203d028(p, 1, 0x151, 0x14000, 0);
            }
        }
        return 1;
    }
    if (state == 6) {
        if (func_0203d060(p, 0, 0, 0xffff, 1) != 0) {
            p[0x60] = 0;
            p[4] = 1;
            func_0203cf74(p, 1, 1);
        }
        return 1;
    }
    if (state == 7) {
        if (func_0203d060(p, 1, 0, 0xffff, 0) != 0) {
            p[4] = 0;
            func_0203cf74(p, 0, 1);
        }
        return 1;
    }
    if (state == 8) {
        if (func_0203d060(p, 0, 0, 0xffff, 1) != 0) {
            *(short *)(p + 0x44) = 0;
            p[4] = 3;
        }
        return 1;
    }
    if (state == 5) {
        r = func_0203cc14(func_020312e4());
        if ((unsigned int)r < 1) {
            return 1;
        }
        if (r == 2) {
            func_0200fbc8(func_02008444());
            r = func_02027d4c(data_0209ab20, 2);
            m = func_0202f624(0xd4);
            if (m != 0) {
                m = func_02067a84(m);
            }
            *(Object **)(p + 0x5c) = (Object *)m;
            ((Object *)m)->slot53(r);
            func_0202f7e8(func_02009900(), *(Object **)(p + 0x5c), 1, 0);
            p[4] = 4;
            p[6] = 0;
            return 1;
        }
        func_0203d028(p, 0, 0x15b, 0x14000, 1);
        p[4] = 8;
        func_0203cc08(func_02008444(), 0);
        p[0x61] = 1;
        return 1;
    }
    if (state == 3) {
        *(short *)(p + 0x44) += 1;
        if (*(short *)(p + 0x44) > 0x2d) {
            func_0203cbe0(func_020312e4(), 0);
        }
        if (*(short *)(p + 0x44) > 0x5a) {
            func_0203cbe0(func_020312e4(), 0);
            func_020521d4(data_0209ab10, 0, 0x3c, 0x100);
            p[4] = 2;
        }
        return 1;
    }
    if (state == 4) {
        p[6]++;
        if (p[6] > 0x10) {
            func_02075f84();
        }
        return 1;
    }
    return 0;
}
}
