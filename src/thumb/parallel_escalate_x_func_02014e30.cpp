class Mark {
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
};

struct Hud {
    unsigned char pad0[0x10];
    unsigned char state;
    unsigned char mode;
    unsigned char pad12[0x1a];
    int w2c;
    unsigned short h30;
    unsigned short h32;
    int w34;
    unsigned char b38;
    unsigned char b39;
    unsigned char pad3a[2];
    unsigned int w3c;
    Mark *w40;
    void *w44;
    void *w48;
    Mark *w4c;
    int w50;
};

extern "C" {
extern unsigned int data_020c9d50[];
extern char data_020cf588[];
extern void *func_02010b7c(void);
extern int func_02015154(void *sys);
extern void func_020229b4(int key);
extern void *func_02008444(void);
extern int func_02015168(void *game);
extern void *func_0201003c(void);
extern int func_02015964(void *mgr);
extern int func_02015aa0(void *mgr);
extern void func_02015a14(void *mgr, int id, int a);
extern void func_02015a24(void *mgr, int id, int a);
extern void func_02015990(void *mgr, int a);
extern int func_02015178(void *mgr);
extern int func_02015188(void *sys);
extern int func_020151a0(void *mgr);
extern int func_02015924(void *mgr);
extern void func_02014e24(Hud *p);
extern void func_02014de4(Mark *mark, int y);
extern void func_02014ddc(Mark *mark, int x);
extern int func_02015a78(void *mgr);
extern int func_020151b0(void *game);
extern int func_02015934(void *mgr);
extern void func_020151d0(Mark *mark, int value);
extern void func_02055e6c(void *obj);
extern void func_0205429c(void *voice);
extern int func_020151e4(void *mgr);
extern int func_020151f4(void *voice);
extern void *func_02009900(void);
extern int func_0202fa8c(void *list, char *name);
extern void *func_02055b38(char *name);
extern void func_02055c28(void *obj, unsigned short id, int a, int b);
extern void func_0202f7e8(void *list, void *obj, int a, int b);
extern void func_020541d8(void *voice, int index, int a, int b);
extern void func_02054254(void *voice, int value);
extern void func_02054278(void *voice, int value);
void func_02014e30(Hud *p);
}

void func_02014e30(Hud *p)
{
    int x;
    int y;

    if (func_02015154(func_02010b7c()) != 0 || (data_020c9d50[0] & 0x200) != 0) {
        if (++p->w3c > 0x3c) {
            func_020229b4(0x200000);
        }
    } else if ((data_020c9d50[0] & 0x200000) != 0) {
        p->w3c++;
    } else {
        p->w3c = 0;
    }
    if (p->w40 != 0) {
        if ((data_020c9d50[0] & 0x200000) != 0 && func_02015168(func_02008444()) != 0 && func_02015964(func_0201003c()) != 0 && func_02015aa0(func_0201003c()) != 0) {
            p->w40->m10();
        } else {
            p->w40->m11();
        }
    }
    switch (p->state) {
    case 0:
        if (p->b39 == 0) {
            func_02015a14(func_0201003c(), p->w50, 1);
        } else {
            func_02015a24(func_0201003c(), p->w50, 0);
        }
        p->mode = 0;
        func_02015990(func_0201003c(), 0);
        p->state = 1;
        break;
    case 1:
        if (func_02015178(func_0201003c()) != 0) {
            return;
        }
        if (func_02015188(func_02010b7c()) != 0 || (data_020c9d50[0] & 0x200) == 0x200) {
            if (func_020151a0(func_0201003c()) == 0) {
                func_020229b4(0x1000);
            }
        } else if (func_02015154(func_02010b7c()) != 0) {
            if (func_020151a0(func_0201003c()) == 0) {
                func_020229b4(0x1000);
            }
        }
        if (func_02015924(func_0201003c()) != 0) {
            func_02014e24(p);
            if (p->w40 == 0) {
                return;
            }
            p->w40->m11();
            return;
        }
        if (p->w4c != 0) {
            if (p->mode == 1) {
                func_02014de4(p->w4c, 0xb7000);
            } else {
                func_02014de4(p->w4c, 0x27000);
            }
            if (func_02015a78(func_0201003c()) != 0) {
                func_02014ddc(p->w4c, 0xde000);
            } else {
                func_02014ddc(p->w4c, 0xf6000);
            }
            if (func_020151b0(func_02008444()) == 0) {
                func_020151d0(p->w4c, func_02015934(func_0201003c()));
            } else {
                p->w4c->m11();
            }
        }
        if (p->w2c != -1) {
            func_02015990(func_0201003c(), 1);
            if (p->mode == 2 && p->w48 != 0) {
                func_02055e6c(p->w48);
                p->w48 = 0;
            }
            p->state = 3;
        } else if (p->w34 != -1) {
            func_02015990(func_0201003c(), 1);
            if (p->mode == 1) {
                if (p->w44 != 0) {
                    func_0205429c(p->w44);
                }
            } else if (p->w48 != 0) {
                func_02055e6c(p->w48);
                p->w48 = 0;
            }
            p->state = 2;
        }
        if (p->w40 == 0) {
            break;
        }
        x = 0x80000;
        y = 0x100000;
        if (func_020151e4(func_0201003c()) == 0) {
            if (p->mode == 1) {
                y = 0xaa000;
            } else {
                if (func_02015a78(func_0201003c()) == 0) {
                    x += 0x1e000;
                }
                y = 0x1a000;
            }
        }
        func_02014ddc(p->w40, x);
        func_02014de4(p->w40, y);
        break;
    case 2:
        if (p->w44 != 0 && func_020151f4(p->w44) == 0) {
            break;
        }
        if (func_0202fa8c(func_02009900(), data_020cf588) != 0) {
            break;
        }
        p->w48 = func_02055b38(data_020cf588);
        if (p->w48 != 0) {
            func_02055c28(p->w48, p->w34, 0, p->b38);
            func_0202f7e8(func_02009900(), p->w48, 7, 0);
        }
        p->w34 = -1;
        p->b38 = 0;
        p->state = 1;
        p->mode = 2;
        break;
    case 3:
        if (func_0202fa8c(func_02009900(), data_020cf588) != 0) {
            break;
        }
        if (p->w44 != 0) {
            if (p->w2c != 0) {
                func_020541d8(p->w44, p->w2c, 0, p->b38);
                switch (p->h30) {
                case 1:
                    func_02054254(p->w44, p->h32);
                    break;
                case 2:
                    func_02054278(p->w44, p->h32);
                    break;
                }
            } else {
                func_0205429c(p->w44);
            }
        }
        p->w2c = -1;
        p->b38 = 0;
        p->state = 1;
        p->mode = 1;
        break;
    }
}
