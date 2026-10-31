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
    virtual void slot45();
    virtual void slot46();
    virtual void slot47();
    virtual void slot48();
    virtual void slot49();
    virtual void slot50();
    virtual void slot51();
    virtual void slot52();
    virtual void slot53(int a, int b, int c, int d, int e, int f, int g);
    virtual void slot54();
    virtual void slot55();
    virtual int slot56();
};
extern "C" {
extern unsigned int data_020c9d50[];
extern void *func_02008444(void);
extern int func_02018db4(void *object);
extern int func_02043bb8(unsigned char *p);
extern void func_020433c4(void *object);
extern void func_020433cc(void *object);
extern unsigned int func_02043644(unsigned char *p);
extern int func_02043664(unsigned char *p);
extern void *func_02010b7c(void);
extern unsigned int func_02015188(void *object);
extern int func_02015154(void *object);
extern int func_0202cb64(void *object);
extern int func_02010b64(void *object);
extern void func_02043c18(unsigned char *p);
extern void func_020229b4(unsigned int value);
extern unsigned int func_020443dc(void *object);
extern int func_02043a10(unsigned char *p);
extern void func_02043a48(unsigned char *p);
extern void func_02043680(unsigned char *p);
extern void *func_0201003c(void);
extern void func_02015b0c(void *object);
extern void *func_0200847c(void);
extern void func_0202a0c0(void *object, int value);
extern int func_020434c4(unsigned char *p, int a, int b);
void func_02043104(unsigned char *p)
{
    short v;
    int a;
    int b;
    Object *o;
    if (func_02018db4(func_02008444()) != 0) {
        return;
    }
    if (func_02043bb8(p) == 0) {
        if (--*(int *)(p + 0x98) == 1) {
            func_020433c4(*(void **)(p + 0x3c));
            func_020433c4(*(void **)(p + 0x40));
        }
    } else {
        if (func_02043644(p) != 0 || func_02043664(p) != 0) {
            *(int *)(p + 0x98) = 0x3c;
            func_020433cc(*(void **)(p + 0x3c));
            func_020433cc(*(void **)(p + 0x40));
        }
    }
    if (func_02015188(func_02010b7c()) != 0 && func_0202cb64(func_02010b7c()) == 0 && p[0xa4] != 0) {
        func_02043c18(p);
    }
    if (func_02015154(func_02010b7c()) != 0 && func_0202cb64(func_02010b7c()) == 0) {
        v = func_02010b64(func_02010b7c());
        *(short *)(p + 0xa0) = 0xc;
        if (p[0xa4] != 0) {
            *(short *)(p + 0x9e) += (short)(v - *(short *)(p + 0x9c));
        }
        *(short *)(p + 0x9c) = v;
        p[0xa4] = 1;
        if (*(short *)(p + 0x9e) > *(short *)(p + 0xa0)) {
            func_020229b4(0x100);
            *(short *)(p + 0x9e) = *(short *)(p + 0x9e) - *(short *)(p + 0xa0);
            *(unsigned short *)(p + 0xa2) = 0;
        } else if (*(short *)(p + 0x9e) < -*(short *)(p + 0xa0)) {
            func_020229b4(0x200);
            *(short *)(p + 0x9e) = *(short *)(p + 0x9e) + *(short *)(p + 0xa0);
            *(unsigned short *)(p + 0xa2) = 0;
        }
        *(unsigned short *)(p + 0xa2) = *(unsigned short *)(p + 0xa2) + 1;
    } else if (p[0xa4] != 0) {
        if (*(short *)(p + 0xa0) == 0xc && *(unsigned short *)(p + 0xa2) > 4 && *(short *)(p + 0x9e) < *(short *)(p + 0xa0)) {
            func_02043c18(p);
        }
        *(short *)(p + 0xa0) = 3;
        if (*(short *)(p + 0x9e) > *(short *)(p + 0xa0)) {
            func_020229b4(0x100);
            *(short *)(p + 0x9e) = *(short *)(p + 0x9e) - 1;
        } else if (*(short *)(p + 0x9e) < -*(short *)(p + 0xa0)) {
            func_020229b4(0x200);
            *(short *)(p + 0x9e) = *(short *)(p + 0x9e) + 1;
        } else {
            func_02043c18(p);
        }
    }
    a = 0;
    b = 0;
    if (func_020443dc(*(void **)(p + 0x3c)) != 0) {
        a = 1;
    } else if (func_020443dc(*(void **)(p + 0x40)) != 0) {
        b = 1;
    }
    if (p[0x10] == 2) {
        if (func_02043a10(p) != 0) {
            return;
        }
        func_02043a48(p);
        p[0x10] = 1;
        p[0x13] = 2;
    }
    if (p[0x13] != 0) {
        p[0x13]--;
        return;
    }
    if (p[0x10] == 0) {
        o = *(Object **)(p + 0x38);
        if (o != 0) {
            o->slot53(0xc0, -2, -0x24, 0x228, 0x22d, 0x228, 0x22d);
            p[0x10] = 1;
            p[0x13] = 0x12;
        }
        func_02043680(p);
        func_02043a48(p);
        return;
    }
    if (p[0x10] == 3) {
        func_02015b0c(func_0201003c());
        p[0x10] = 4;
        return;
    }
    o = *(Object **)(p + 0x38);
    if (o != 0 && o->slot56() != 0) {
        func_020229b4(0x2000);
    }
    func_02043680(p);
    if ((data_020c9d50[1] & 0x2000) != 0) {
        func_0202a0c0(func_0200847c(), 3);
        (*(Object **)(p + 0x38))->slot55();
        *(Object **)(p + 0x38) = 0;
        p[0x10] = 3;
        p[0x13] = 0x14;
        return;
    }
    if (func_020434c4(p, (data_020c9d50[0] & 0x100) != 0, (data_020c9d50[0] & 0x200) != 0) == 0) {
        func_020434c4(p, a, b);
    }
}
}
