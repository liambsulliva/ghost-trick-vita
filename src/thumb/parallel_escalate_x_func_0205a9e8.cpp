class Sprite {
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
    virtual void s15(int y);
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
    virtual void s42(int id, int a, int b, int c, int d, int e, int f);
};

class Panel {
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
    virtual int s56();
};

struct Card {
    unsigned char pad0[0x10];
    unsigned char state;
    unsigned char pad11[2];
    unsigned char wait;
    unsigned char pad14[0x14];
    unsigned short count;
    unsigned short h2a;
    short h2c;
    signed char cur;
    signed char pick;
    unsigned char b30;
    unsigned char b31;
    unsigned char b32;
    unsigned char b33;
    Panel *panel;
    unsigned char pad38[8];
    Sprite *marker;
    void *slots[4];
};

struct Repeat {
    unsigned char on;
    unsigned char count;
};

extern "C" {
extern unsigned int data_020c9d50[];
extern struct Repeat data_020cf958;
extern char data_020cece8[];
extern char data_0209da64[];
extern char data_0209da68[];
extern char data_0209da70[];
extern char data_0209da78[];
extern void func_0205b960(void *slot);
extern void func_0205b250(Card *p);
extern Panel *func_0204ed1c(char *name);
extern void *func_02009900(void);
extern void func_0202f7e8(void *list, void *obj, int a, int b);
extern void *func_02010b7c(void);
extern int func_02015188(void *sys);
extern int func_02015154(void *sys);
extern int func_0202cb64(void *sys);
extern void func_020229b4(unsigned int key);
extern signed char func_0205b044(Card *p);
extern int func_0205b3f0(Card *p, unsigned char n);
extern void func_0205b0b8(Card *p);
extern void func_0205b110(Card *p);
extern void *func_0200847c(void);
extern void func_0202a0c0(void *sys, int id);
extern void func_0205b994(void *slot);
extern void *func_02008444(void);
extern void func_0205ad20(void *game, int index);
extern int func_0205ad2c(void *slot);
extern void func_0205b36c(Card *p, int value);
extern void func_0205b3b4(Card *p, int value);
extern void func_0200da04(char *a, char *b, char *c, char *d);
extern void *func_0201003c(void);
extern void func_02015be4(void *mgr);
extern void func_0205b228(Card *p);
extern unsigned int func_0205ad34(void *slot);
void func_0205a9e8(Card *p);
}

void func_0205a9e8(Card *p)
{
    int i;
    int n;
    int x;
    unsigned int k;
    Panel *panel;

    if (p->wait != 0) {
        p->wait--;
        return;
    }
    switch (p->state) {
    case 1:
        p->state = 2;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
        i = p->state - 2;
        if (p->slots[i] != 0) {
            if (i < p->h2a) {
                func_0205b960(p->slots[i]);
            }
            p->state++;
        } else {
            p->state = 6;
        }
        p->wait = 0xc;
        break;
    case 6:
        if (p->b31 == 0) {
            p->state = 7;
        } else {
            p->state = 0xe;
            p->wait += 0x10;
        }
        p->wait += 8;
        break;
    case 7:
        if (p->b33 != 0) {
            func_0205b250(p);
        }
        if (p->b32 != 0) {
            panel = func_0204ed1c(data_020cece8);
            p->panel = panel;
            if (panel != 0) {
                func_0202f7e8(func_02009900(), panel, 1, 0);
            }
            p->wait = 0x12;
        }
        p->state = 8;
        break;
    case 8:
        p->marker->s10();
        p->state = 9;
        break;
    case 9:
        if (func_02015188(func_02010b7c()) != 0 && func_0202cb64(func_02010b7c()) == 0) {
            if (p->panel != 0 && p->panel->s56() != 0) {
                func_020229b4(0x2000);
            }
            n = func_0205b044(p);
            if (n != -1) {
                p->cur = func_0205b3f0(p, n);
                p->pick = n;
                x = (1 - p->pick) * 0x2a;
                p->marker->s15((x + p->h2c) << 12);
                if (p->pick < p->count) {
                    p->state = 0xa;
                }
                break;
            }
        }
        if (p->b30 != 0 && func_02015154(func_02010b7c()) != 0 && func_0202cb64(func_02010b7c()) == 0) {
            func_0205b044(p);
        }
        if ((data_020c9d50[1] & 0x1000) != 0) {
            if (p->pick < p->count) {
                p->state = 0xa;
            }
            break;
        }
        if ((data_020c9d50[1] & 0x2000) != 0) {
            if (p->panel != 0) {
                p->panel->s55();
                p->panel = 0;
                func_0202a0c0(func_0200847c(), 3);
                p->state = 0xc;
                p->wait = 0x18;
            }
            break;
        }
        if (p->b30 != 0 && data_020cf958.on != 0) {
            if ((data_020c9d50[0] & 0x200) != 0) {
                func_0205b0b8(p);
                p->wait = 2;
            } else if ((data_020c9d50[0] & 0x100) != 0) {
                func_0205b110(p);
                p->wait = 2;
            } else {
                data_020cf958.on = 0;
                data_020cf958.count = 0;
            }
            break;
        }
        if (p->count <= 1) {
            break;
        }
        if ((data_020c9d50[3] & 0x200) != 0) {
            func_0205b0b8(p);
        } else if ((data_020c9d50[3] & 0x100) != 0) {
            func_0205b110(p);
        }
        if ((data_020c9d50[0] & 0x200) != 0 || (data_020c9d50[0] & 0x100) != 0) {
            data_020cf958.count++;
            if (data_020cf958.count > 8) {
                data_020cf958.on = 1;
            }
        }
        break;
    case 10:
        p->marker->s42(0xc2, 0, 1, 0, 1, -1, 0);
        func_0205b994(p->slots[p->cur]);
        func_0202a0c0(func_0200847c(), 2);
        p->state = 0xb;
        p->wait = 0x10;
        break;
    case 11:
        func_0205ad20(func_02008444(), p->cur);
        func_0205b36c(p, func_0205ad2c(p->slots[p->cur]));
        func_0205b3b4(p, func_0205ad2c(p->slots[p->cur]));
        if (p->panel != 0) {
            p->panel->s54();
            p->panel = 0;
        }
        p->state = 0xd;
        break;
    case 12:
        func_0200da04(data_0209da64, data_0209da68, data_0209da70, data_0209da78);
        p->state = 0xd;
        break;
    case 13:
        func_02015be4(func_0201003c());
        break;
    case 14:
        func_0205b228(p);
        p->state = 0xf;
        break;
    case 15:
        for (k = 0; k < p->count; k++) {
            if (func_0205ad34(p->slots[k]) != 0) {
                return;
            }
        }
        p->state = 0x10;
        break;
    case 16:
        p->state = 7;
        p->wait = 4;
        break;
    }
}
