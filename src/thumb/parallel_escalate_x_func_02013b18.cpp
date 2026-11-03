class Obj {
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
};

extern "C" {
extern char data_02084310[];
extern char data_020cf93c[];
extern char data_0208431c[];
extern char data_02084328[];
extern char data_02084334[];
extern char data_02084340[];
extern char data_0208434c[];
extern char data_0208435c[];
extern char data_0208436c[];
extern char data_0208437c[];
extern char data_02084388[];
extern char data_02084394[];
extern char data_020cff28[];
extern char data_020843a4[];
extern char data_020843b0[];
extern char data_020cf898[];
extern char data_020843b8[];
extern char data_020843cc[];
extern char data_020ce91c[];
extern char data_020843e0[];
extern char data_020843f8[];
extern char data_02084410[];
extern char data_02084428[];
extern char data_020ce7d4[];
extern char data_02084438[];
extern char data_0208444c[];
extern char data_0208445c[];
extern char data_020841dc[];
extern char data_020841e8[];
extern char data_02084468[];
extern char data_02084228[];
extern char data_02084470[];
extern char data_020cfaf0[];
extern char data_0208447c[];
extern char data_02084488[];
extern char data_02084494[];
extern char data_020844a8[];
extern char data_020844bc[];
extern char data_020844d0[];
extern char data_020844e4[];
extern char data_020844f4[];
extern char data_02084508[];
extern char data_02084520[];
extern char data_02084538[];
extern char data_02084548[];
extern char data_0208455c[];
extern char data_02084574[];
extern char data_0208458c[];
extern char data_0208459c[];
extern char data_020845b0[];
extern char data_020845c4[];
extern char data_020845d8[];
extern char data_020845ec[];
extern char data_02084600[];
extern char data_02084614[];
extern char data_02084628[];
extern char data_02084640[];
extern char data_02084650[];
extern char data_020cd96c[];
extern char data_02084660[];
extern char data_02084670[];
extern char data_020ce1e8[];
extern char data_02084684[];
extern char data_02084694[];
extern char *func_02013514(int id);
extern int func_020790c8(char *a, char *b);
extern void *func_02009900(void);
extern void func_0202fad4(void *system, char *name);
extern void *func_02008444(void);
extern void func_0200ee54(void *game, int value);
extern void func_020141f4(void *game);
extern void func_020110ec(void *game);
extern void func_0201192c(void *game, int value);
extern void func_02010384(void *game, int value);
extern void func_0201420c(void *game, int value);
extern void func_02011598(void *game);
extern void *func_02011264(void);
extern void *func_0201123c(void *list, int index);
extern int func_02022fa4(void *object, char *name);
extern void func_02011404(void *game);
extern void *func_02008860(void);
extern void func_0200c82c(void *sys, int value);
extern void func_02011340(void *game);
extern void func_0201135c(void *game);
extern void func_020521d4(char *name, int a, int b, int c);
extern void *func_02009c1c(void);
extern void func_0202f3a4(void *system, int a, int b);
extern void func_0202fbcc(unsigned int *it, int kind);
extern int func_0200eac4(unsigned int *it);
extern void *func_0200eab4(unsigned int *it);
extern Obj *func_0200eab8(unsigned int *it);
extern void func_0200eabc(unsigned int *it, int a);
extern int func_02008828(void *object, char *kind);
extern char *func_02014220(Obj *object);
extern void func_02059020(Obj *object);
extern void *func_0202fb48(void *system, char *name, int index);
extern void func_02014228(void *object, int value);
extern void func_02015758(int value);
extern void func_02046ec0(void *object);
extern void func_02046ed0(void *object);
extern void func_02014230(void *game, int value);
extern void func_0201424c(void *game);
extern void func_02014260(void *game);
extern void func_02014270(void *game);
extern void func_0205e2d0(void *object);
extern void func_0205e2dc(void *object);
extern void func_02010028(void *game, int value);
extern void func_02014284(void *game, int value);
extern void func_02014298(void *game, int value);
extern void func_020142ac(void *game, int value);
extern void func_020142c0(void *game, int value);
extern void func_020142d4(void *game, int value);
extern void func_020142e8(void *game, int value);
extern void func_020142fc(void *game, int value);
extern void func_02014310(void *game, int value);
extern void func_02014324(void *game, int value);
extern void func_02014338(void *game, int value);
extern void func_02010858(void *game);
extern void func_020117a8(void *game, int value);
extern void *func_02022fc0(void *object, char *name);
extern void func_0201434c(void *object);
extern void func_02014358(int value);
extern void func_0203f208(void *object);
extern void func_02022f34(void *object, char *name, int index);

void func_02013b18(void *a, void *b, int id)
{
    char *name;
    unsigned int it[4];
    Obj *obj;
    void *found;

    name = func_02013514(id);
    if (func_020790c8(name, data_02084310) == 0) {
        func_0202fad4(func_02009900(), data_020cf93c);
        func_0200ee54(func_02008444(), 0);
        func_020141f4(func_02008444());
        func_020110ec(func_02008444());
    } else if (func_020790c8(name, data_0208431c) == 0) {
        func_0202fad4(func_02009900(), data_020cf93c);
        func_0200ee54(func_02008444(), 0);
    } else if (func_020790c8(name, data_02084328) == 0) {
        func_0201192c(func_02008444(), 3);
        func_02010384(func_02008444(), 0);
    } else if (func_020790c8(name, data_02084334) == 0) {
        func_0201420c(func_02008444(), 1);
    } else if (func_020790c8(name, data_02084340) == 0) {
        func_0201420c(func_02008444(), 0);
    } else if (func_020790c8(name, data_0208434c) == 0) {
        func_02011598(func_02008444());
    } else if (func_020790c8(name, data_0208435c) == 0) {
        if (func_02022fa4(func_0201123c(func_02011264(), 1), data_0208436c) == 0) {
            func_02011404(func_02008444());
            func_0200c82c(func_02008860(), 1);
            func_02011340(func_02008444());
            func_0201135c(func_02008444());
        }
    } else if (func_020790c8(name, data_0208437c) == 0) {
        func_020521d4(data_02084388, 1, 0xf, 0x100);
        func_020110ec(func_02008444());
    } else if (func_020790c8(name, data_02084394) == 0) {
        func_0202fad4(func_02009900(), data_020cff28);
    } else if (func_020790c8(name, data_020843a4) == 0) {
        func_0202f3a4(func_02009c1c(), 4, 0);
        func_0202fbcc(it, 6);
        while (func_0200eac4(it) != 0) {
            if (func_02008828(func_0200eab4(it), data_020cf898) != 0) {
                obj = func_0200eab8(it);
                if (func_020790c8(func_02014220(obj), data_020843b0) == 0) {
                    func_02059020(obj);
                    obj->slot15();
                    return;
                }
            }
            func_0200eabc(it, 0);
        }
    } else if (func_020790c8(name, data_020843b8) == 0) {
        func_0202f3a4(func_02009c1c(), 4, 0);
    } else if (func_020790c8(name, data_020843cc) == 0) {
        found = func_0202fb48(func_02009900(), data_020ce91c, -1);
        if (found != 0) {
            func_02014228(found, 1);
        }
    } else if (func_020790c8(name, data_020843e0) == 0) {
        found = func_0202fb48(func_02009900(), data_020ce91c, -1);
        if (found != 0) {
            func_02014228(found, 0);
        }
    } else if (func_020790c8(name, data_020843f8) == 0) {
        func_02015758(1);
    } else if (func_020790c8(name, data_02084410) == 0) {
        func_02015758(0);
    } else if (func_020790c8(name, data_02084428) == 0) {
        found = func_0202fb48(func_02009900(), data_020ce7d4, -1);
        if (found != 0) {
            func_02046ec0(found);
        }
    } else if (func_020790c8(name, data_02084438) == 0) {
        found = func_0202fb48(func_02009900(), data_020ce7d4, -1);
        if (found != 0) {
            func_02046ed0(found);
        }
    } else if (func_020790c8(name, data_0208444c) == 0) {
        func_02014230(func_02008444(), 0);
    } else if (func_020790c8(name, data_0208445c) == 0) {
        func_02014230(func_02008444(), 1);
    } else if (func_020790c8(name, data_020841dc) == 0) {
        func_0201424c(func_02008444());
    } else if (func_020790c8(name, data_020841e8) == 0) {
        func_02014260(func_02008444());
    } else if (func_020790c8(name, data_02084468) == 0) {
        func_02014270(func_02008444());
    } else if (func_020790c8(name, data_02084228) == 0) {
        func_02014260(func_02008444());
    } else if (func_020790c8(name, data_02084470) == 0) {
        found = func_0202fb48(func_02009900(), data_020cfaf0, -1);
        if (found != 0) {
            func_0205e2d0(found);
        }
    } else if (func_020790c8(name, data_0208447c) == 0) {
        found = func_0202fb48(func_02009900(), data_020cfaf0, -1);
        if (found != 0) {
            func_0205e2dc(found);
        }
    } else if (func_020790c8(name, data_02084488) == 0) {
        func_02010028(func_02008444(), 1);
    } else if (func_020790c8(name, data_02084494) == 0) {
        func_02014284(func_02008444(), 1);
    } else if (func_020790c8(name, data_020844a8) == 0) {
        func_02014284(func_02008444(), 0);
    } else if (func_020790c8(name, data_020844bc) == 0) {
        func_02014298(func_02008444(), 1);
    } else if (func_020790c8(name, data_020844d0) == 0) {
        func_02014298(func_02008444(), 0);
    } else if (func_020790c8(name, data_020844e4) == 0) {
        func_020142ac(func_02008444(), 1);
    } else if (func_020790c8(name, data_020844f4) == 0) {
        func_020142ac(func_02008444(), 0);
    } else if (func_020790c8(name, data_02084508) == 0) {
        func_020142c0(func_02008444(), 1);
    } else if (func_020790c8(name, data_02084520) == 0) {
        func_020142c0(func_02008444(), 0);
    } else if (func_020790c8(name, data_02084538) == 0) {
        func_020142d4(func_02008444(), 0);
    } else if (func_020790c8(name, data_02084548) == 0) {
        func_020142d4(func_02008444(), 1);
    } else if (func_020790c8(name, data_0208455c) == 0) {
        func_020142e8(func_02008444(), 1);
    } else if (func_020790c8(name, data_02084574) == 0) {
        func_020142e8(func_02008444(), 0);
    } else if (func_020790c8(name, data_0208458c) == 0) {
        func_020142fc(func_02008444(), 0);
    } else if (func_020790c8(name, data_0208459c) == 0) {
        func_020142fc(func_02008444(), 1);
    } else if (func_020790c8(name, data_020845b0) == 0) {
        func_02014310(func_02008444(), 0);
    } else if (func_020790c8(name, data_020845c4) == 0) {
        func_02014310(func_02008444(), 1);
    } else if (func_020790c8(name, data_020845d8) == 0) {
        func_02014324(func_02008444(), 1);
    } else if (func_020790c8(name, data_020845ec) == 0) {
        func_02014324(func_02008444(), 0);
    } else if (func_020790c8(name, data_02084600) == 0) {
        func_02014338(func_02008444(), 1);
    } else if (func_020790c8(name, data_02084614) == 0) {
        func_02014338(func_02008444(), 0);
    } else if (func_020790c8(name, data_02084628) == 0) {
        func_02010858(func_02008444());
    } else if (func_020790c8(name, data_02084640) == 0) {
        func_020117a8(func_02008444(), 0);
    } else if (func_020790c8(name, data_02084650) == 0) {
        found = func_02022fc0(func_0201123c(func_02011264(), 0), data_020cd96c);
        if (found != 0) {
            func_0201434c(found);
        }
    } else if (func_020790c8(name, data_02084660) == 0) {
        func_02014358(1);
    } else if (func_020790c8(name, data_02084670) == 0) {
        found = func_02022fc0(func_0201123c(func_02011264(), 1), data_020ce1e8);
        if (found != 0) {
            func_0203f208(found);
        }
    } else if (func_020790c8(name, data_02084684) == 0) {
        func_020521d4(data_02084388, 1, 0xf, 0x100);
        func_02022f34(func_0201123c(func_02011264(), 1), data_02084694, 0);
    }
}
}
