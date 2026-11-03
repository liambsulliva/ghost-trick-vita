class Part {
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
    virtual void slot13(int x, int y, int z);
    virtual void slot14(int value);
    virtual void slot15(int value);
    virtual void slot16();
    virtual void *slot17();
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
};

class Stage {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8(int value);
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
    virtual void slot66();

    unsigned char pad4[0xb0];
    unsigned char flip;
    unsigned char padb5[3];
    Part *main;
    Part *pair[2];
    Part *third;
    int step;
    int timer;
    unsigned char finish;
    unsigned char cancel;
    unsigned char said_a;
    unsigned char said_b;
    int kx;
    int ky;
    int kz;
    int spread;
    int pade4;
    unsigned char done;
    unsigned char pade9;
    unsigned char skip;
    unsigned char padeb[5];
    int pos[3];
};

extern "C" {
extern void *func_02008444(void);
extern void func_02010f0c(void *manager, int id, int value);
extern Part *func_02063b48(void *heap);
extern void func_020153d8(Part *part, Stage *stage);
extern void func_0203575c(Part *part, Stage *stage);
extern void *func_02009900(void);
extern void func_0202f7e8(void *system, Part *part, int a, int b);
extern int func_02049ab8(void *object);
extern int func_02010d44(void *manager);
extern int func_02010d74(void *manager);
extern void func_02010d00(void *manager);
extern void func_020083b0(int *dst, void *src);
extern void func_0200da04(char *a, char *b, char *c, char *d);
extern char data_020cfd28[];
extern char data_0209b7f4[];
extern char data_0209b858[];
extern char data_0209b868[];
extern char data_0209b86c[];
extern char data_0209b810[];
void func_02049754(Stage *p);
}

void func_02049754(Stage *p)
{
    unsigned int i;
    int x;
    int y;
    int z;
    int w;

    func_02010f0c(func_02008444(), 0, 0);
    func_02010f0c(func_02008444(), 1, 0);
    func_02010f0c(func_02008444(), 3, 0);
    func_02010f0c(func_02008444(), 4, 0);
    func_02010f0c(func_02008444(), 6, 0);
    func_02010f0c(func_02008444(), 7, 0);
    p->timer++;
    if (p->main == 0 && p->timer > 12) {
        p->main = func_02063b48(data_020cfd28);
        p->main->slot33(0x472);
        p->main->slot11();
        func_020153d8(p->main, p);
        func_0203575c(p->main, p);
        func_0202f7e8(func_02009900(), p->main, 7, 0);
    }
    if (p->timer > 0x80) {
        p->timer = 0x80;
    }
    if (p->main == 0) {
        if (p->timer > 12) {
            p->slot8(0);
        }
        return;
    }
    for (i = 0; i < 2; i++) {
        if (p->pair[i] == 0) {
            p->slot8(0);
            return;
        }
    }
    if (p->third == 0) {
        p->slot8(0);
        return;
    }
    if (func_02049ab8(p->main) == 0) {
        return;
    }
    if (func_02010d44(func_02008444()) != 0) {
        if (p->skip == 0) {
            p->main->slot10();
        }
        p->third->slot10();
        for (i = 0; i < 2; i++) {
            p->pair[i]->slot10();
        }
    } else {
        if (func_02010d74(func_02008444()) != 0) {
            p->done = 1;
            func_02010d00(func_02008444());
        }
        return;
    }
    if (p->step <= 0x10 && p->said_a == 0 && p->said_b == 0 && p->finish == 0 && p->cancel == 0) {
        x = p->kx * p->step;
        y = p->ky * p->step;
        z = p->kz * p->step - 0x80000;
        if (p->flip) {
            z = -z;
        }
        p->main->slot13(x, y, 0);
        p->third->slot14(z);
        w = p->kz * p->step - 0xc4000;
        if (p->flip) {
            w = -w;
        }
        for (i = 0; i < 2; i++) {
            p->pair[i]->slot14(w);
        }
        func_020083b0(p->pos, p->main->slot17());
        p->step++;
        return;
    }
    if (p->finish != 0 || p->cancel != 0) {
        p->slot66();
        if (p->step > 0x10 || p->said_b != 0) {
            if (p->said_b == 0) {
                func_0200da04(data_0209b7f4, data_0209b858, data_0209b868, data_0209b810);
                p->said_b = 1;
            }
            if (p->finish != 0) {
                p->slot8(0);
            }
        } else {
            for (i = 0; i < 2; i++) {
                p->pair[i]->slot15(((0x30 - i * 0x60) << 12) + (1 - i * 2) * (p->spread * p->step));
            }
            p->step++;
        }
    } else {
        p->slot66();
        if (p->said_a == 0) {
            func_0200da04(data_0209b7f4, data_0209b858, data_0209b86c, data_0209b810);
            p->said_a = 1;
        }
    }
}
