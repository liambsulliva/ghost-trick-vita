
class Obj0205cdac {
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
    virtual void slot12(void *v);
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual void slot21();
    virtual void slot22(int x, int y, int z);
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
    virtual void slot33(int v);
    virtual void slot34();
    virtual void slot35(signed char v);
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
};

struct Entry0205cdac {
    int obj;
    int rest[4];
};

extern "C" {
extern unsigned char data_020cff60[];
extern unsigned int *func_02008444(void);
extern int func_0205d0c0(void *object);
extern void func_0205d534(void *object);
extern int func_02036f3c(int object);
extern int func_02010194(void *object);
extern int func_0201f0e4(int a, int b, int c, int d, int e, int f);
extern void func_0205d844(void *object, void *a, void *b, void *c, unsigned short *d, void *e, void *f, void *g);
extern unsigned int func_0206f28c(int a, int b);

void func_0205cdac(unsigned char *p)
{
    int v;
    short t;
    unsigned int i;
    int e;
    int r;
    int scale;
    int d;
    short s;
    unsigned short angle[2];
    int g;
    unsigned char *gp;
    unsigned char *gq;
    int v1;
    int v2;

    if (func_0205d0c0(func_02008444()) != 0) {
        ((Obj0205cdac *)p)->slot33(0x3c0);
        (*(Obj0205cdac **)(p + 0x80))->slot33(0x3c0);
        ((Obj0205cdac *)(*(unsigned char **)(p + 0x80) + 0xc8))->slot33(0x380);
    }
    else {
        ((Obj0205cdac *)p)->slot33(0x940);
        (*(Obj0205cdac **)(p + 0x80))->slot33(0x940);
        ((Obj0205cdac *)(*(unsigned char **)(p + 0x80) + 0xc8))->slot33(0x8d0);
    }
    gp = data_020cff60;
    gq = (unsigned char *)(int)(long long)(int)gp;
    *(int *)(p + 0x4c) = 0x29;
    *(int *)(p + 0x48) = *(int *)(p + 0x48) + *(int *)(p + 0x4c) * (gp[1] + 1);
    *(short *)(p + 0x50) = *(short *)(p + 0x50) + (short)(*(short *)(p + 0x52) * (gq[1] + 1));
    if (*(short *)(p + 0x50) > 0x5d) {
        *(short *)(p + 0x50) = 0x5d;
        *(short *)(p + 0x52) *= -1;
    }
    if (*(short *)(p + 0x50) < 0x48) {
        *(short *)(p + 0x50) = 0x48;
        *(short *)(p + 0x52) *= -1;
    }
    if (*(int *)(p + 0x48) > 0xb33) {
        *(int *)(p + 0x48) = 0xb33;
        *(int *)(p + 0x4c) *= -1;
    }
    if (*(int *)(p + 0x48) < 0x800) {
        *(int *)(p + 0x48) = 0x800;
        *(int *)(p + 0x4c) *= -1;
    }
    s = *(short *)(p + 0x58);
    if (s > 0) {
        *(short *)(p + 0x58) = s - 1;
    }
    else if (s == 0) {
        *(int *)(p + 0x68) = 0;
        *(short *)(p + 0x58) = -1;
        func_0205d534(p);
    }
    *(int *)(p + 0x64) = 0;
    if (*(unsigned int *)(p + 0x5c) == 0) {
        func_0205d534(p);
    }
    for (i = 0; i < *(unsigned int *)(p + 0x5c); i++) {
        if (func_02036f3c((*(struct Entry0205cdac **)(p + 0x6c))[i].obj) != 0 &&
            (e = (*(struct Entry0205cdac **)(p + 0x6c))[i].obj, e != *(int *)(p + 0x68))) {
            continue;
        }
        e = (*(struct Entry0205cdac **)(p + 0x6c))[i].obj;
        if (e == func_02010194(func_02008444())) {
            *(int *)(p + 0x64) = e;
            break;
        }
    }
    if (*(int *)(p + 0x8c) != 0) {
        r = func_0201f0e4(0, 0x8000, 0x1800, *(int *)(p + 0x90), ++(*(int *)(p + 0x8c)) - 1, 0x18);
        angle[1] = r;
        func_0205d844(p, p + 0xd4, p + 0xbc, p + 0xe0, &angle[1], p + 0xc8, p + 0xcc, p + 0xd0);
        angle[0] = r;
        func_0205d844(p, p + 0x114, p + 0xf0, p + 0x120, &angle[0], p + 0x108, p + 0x10c, p + 0x110);
        if (*(Obj0205cdac **)(p + 0x9c) != 0) {
            (*(Obj0205cdac **)(p + 0x9c))->slot12(p + 0xd4);
        }
        if (*(Obj0205cdac **)(p + 0xa0) != 0) {
            (*(Obj0205cdac **)(p + 0xa0))->slot12(p + 0x114);
        }
        if (*(int *)(p + 0x8c) >= 0x19) {
            *(int *)(p + 0x94) = 0;
            *(int *)(p + 0x98) = 0;
            *(int *)(p + 0x8c) = 0;
            if (*(Obj0205cdac **)(p + 0x9c) != 0) {
                (*(Obj0205cdac **)(p + 0x9c))->slot51();
                *(Obj0205cdac **)(p + 0x9c) = 0;
            }
            if (*(Obj0205cdac **)(p + 0xa0) != 0) {
                (*(Obj0205cdac **)(p + 0xa0))->slot51();
                *(Obj0205cdac **)(p + 0xa0) = 0;
            }
            func_0205d534(p);
        }
    }
    if (*(int *)(p + 0x64) != 0) {
        *(int *)(p + 0x54) = *(int *)(p + 0x54) + 0x266;
        if (*(int *)(p + 0x54) > 0x2000) {
            *(int *)(p + 0x54) = 0x2000;
        }
    }
    else {
        *(int *)(p + 0x54) = *(int *)(p + 0x54) - (data_020cff60[1] + 1) * 0x11f;
        if (*(int *)(p + 0x54) < 0x1000) {
            *(int *)(p + 0x54) = 0x1000;
        }
    }
    scale = *(int *)(p + 0x48);
    func_0206f28c(scale, *(int *)(p + 0x54));
    d = *(short *)(p + 0x50) / 3;
    (*(Obj0205cdac **)(p + 0x80))->slot22(scale, scale, scale);
    (*(Obj0205cdac **)(p + 0x80))->slot35(d);
}
}

