class Res {
public:
    virtual void r0();
    virtual void r1();
    virtual void *r2(int size, void *name);
};

struct Pair {
    int a;
    int b;
};

struct Panel {
    unsigned char pad000[4];
    unsigned char alarm[0x2c];
    int w30;
    int w34;
    int w38;
    int w3c;
    int w40;
    unsigned int w44;
    int w48;
    unsigned char fileA[0x48];
    unsigned char fileB[0x48];
    int wdc;
    int we0;
    int we4;
    int we8;
    unsigned int wec;
    unsigned int wf0;
    unsigned int wf4;
    unsigned int wf8;
    unsigned char pad0fc[0x9c4 - 0xfc];
    int w9c4;
    int w9c8;
    int w9cc;
    int w9d0;
    int w9d4;
    int w9d8;
    int w9dc;
    int w9e0;
    int w9e4;
    struct Pair pairs[16];
    int wa68;
    int wa6c;
    int wa70;
    unsigned char pada74[4];
    int *wa78;
    int wa7c;
    unsigned int wa80;
    unsigned char pada84[8];
    int *wa8c;
    int wa90;
    unsigned int wa94;
    unsigned char pada98[8];
    void *waa0;
    void *waa4;
    unsigned char aa8_low:2;
    unsigned char first_b:1;
    unsigned char second_b:1;
    unsigned char aa8_b4:1;
    unsigned char aa8_b5:1;
    unsigned char aa8_b6:1;
    unsigned char aa8_b7:1;
    unsigned char aa9_b0:1;
    unsigned char aa9_b1:1;
    unsigned char aa9_b2:1;
    unsigned char aa9_b3:1;
    unsigned char aa9_b4:1;
    unsigned char first_a:1;
    unsigned char second_a:1;
    unsigned char aa9_b7:1;
    unsigned char mode:3;
    unsigned char kind:2;
};

extern "C" {
extern char data_ov004_020e45f4;
extern unsigned long long data_020bbb58;
extern unsigned int data_020bbb54;
extern int func_ov004_020d61f0(void);
extern void func_ov004_020d6200(int irq);
extern void *func_02007bd8(void);
extern Res *func_02007b70(void *a);
extern void func_0201f2e8(void *a);
extern void *func_02031254(int size);
extern void *func_0200c504(void *object);
extern void func_0201e144(void *object, void *data, int size, int mode);
extern int func_ov004_020d6210(void);
extern int func_ov004_020d621c(void);
extern int func_ov004_020d6228(void);
extern unsigned int func_02074f48(void);
extern unsigned int func_ov004_020d4d48(void);
extern unsigned int func_ov004_020d4d54(void);
extern void func_ov004_020d4d5c(int a, unsigned int b);
extern void func_ov004_020d4d7c(int a, unsigned int b);
extern void func_0206c440(void *file);
extern int func_0206c7d0(void *file, int source);
extern int func_ov004_020d3bdc(void *file, int count);
extern unsigned int func_ov004_020d3d2c(int handle);
extern unsigned int func_ov004_020d3d44(int handle);
extern unsigned short func_ov004_020d3cfc(int handle);
extern int func_ov004_020d3d5c(int handle);
extern unsigned int func_ov004_020d3ce4(int handle);
extern unsigned int func_ov004_020d3d14(int handle);
extern int func_ov004_020d6b58(int size);
extern int func_ov004_020d57b0(int a, int b);
extern void func_020747e4(void);
extern void func_020754f4(void *alarm);
extern unsigned long long func_020753bc(void);
extern void func_02075698(void *alarm, unsigned long long start, unsigned long long period, void (*handler)(void), void *arg);
extern void func_ov004_020d6bf8(void);
extern int func_ov004_020d3dec(int handle, int value);
extern int func_ov004_020d3d8c(int handle, int value);

int func_ov004_020d5c8c(struct Panel *p)
{
    int flag;
    int i;
    void *object;
    unsigned int n;
    unsigned int a;
    unsigned int b;
    unsigned long long tick;

    p->aa8_b5 = 0;
    p->aa8_b6 = 1;
    p->w9d8 = 0;
    p->w9dc = 0;
    p->w9e0 = 0;
    p->w9e4 = 0;
    for (i = 0; i < 16; i++) {
        p->pairs[i].a = 0;
        p->pairs[i].b = 0;
    }
    p->wa68 = 0;
    if (p->aa9_b3) {
        flag = 0;
        if (func_ov004_020d61f0() != 0) {
            flag = 1;
        }
        if (!p->aa9_b4) {
            p->waa0 = func_02007b70(func_02007bd8())->r2(0x80000, &data_ov004_020e45f4);
        }
        if (p->waa0 == 0) {
            func_ov004_020d6200(flag);
            func_0201f2e8(func_02007bd8());
            return 0;
        }
        object = func_02031254(0x3c);
        if (object != 0) {
            object = func_0200c504(object);
        }
        p->waa4 = object;
        if (p->w9c4 != 0 && p->w9c8 != 0) {
            func_0201e144(p->waa4, p->waa0, 0x100000, 0x20);
        } else {
            func_0201e144(p->waa4, p->waa0, 0x80000, 0x20);
        }
        func_ov004_020d6200(flag);
    }
    if (p->aa9_b1) {
        n = func_ov004_020d6210() - func_ov004_020d621c();
        if (n >= func_ov004_020d4d48()) {
            n = func_ov004_020d4d48();
        }
        func_ov004_020d4d5c(func_ov004_020d621c(), n);
    } else {
        func_ov004_020d4d5c(func_ov004_020d621c(), 0);
    }
    if (p->aa9_b2) {
        n = func_02074f48() - func_ov004_020d6228();
        if (n >= func_ov004_020d4d54()) {
            n = func_ov004_020d4d54();
        }
        func_ov004_020d4d7c(func_ov004_020d6228(), n);
    } else {
        func_ov004_020d4d7c(func_ov004_020d6228(), 0);
    }
    if (p->w9c4 != 0) {
        func_0206c440(p->fileA);
        if (func_0206c7d0(p->fileA, p->w9c4) == 0) {
            return 0;
        }
    }
    if (p->w9c8 != 0) {
        func_0206c440(p->fileB);
        if (func_0206c7d0(p->fileB, p->w9c8) == 0) {
            return 0;
        }
    }
    p->wdc = 6;
    if (p->w9c4 != 0) {
        p->w9cc = func_ov004_020d3bdc(p->fileA, p->wdc);
        if (p->w9cc == 0) {
            return 0;
        }
        p->mode |= 1;
    }
    if (p->w9c8 != 0) {
        p->w9d0 = func_ov004_020d3bdc(p->fileB, p->wdc);
        if (p->w9d0 == 0) {
            return 0;
        }
        p->mode |= 2;
    }
    p->aa8_b6 = 0;
    p->we0 = 0;
    p->we4 = 0;
    p->we8 = 0;
    p->wa7c = 0;
    p->wa90 = 0;
    if (p->w9cc != 0) {
        p->wec = func_ov004_020d3d2c(p->w9cc);
        p->wa80 = func_ov004_020d3d44(p->w9cc);
    }
    if (p->w9d0 != 0) {
        p->wf0 = func_ov004_020d3d2c(p->w9d0);
        p->wa94 = func_ov004_020d3d44(p->w9d0);
    }
    p->kind = 0;
    if (p->mode & 1) {
        p->w9d4 = p->w9cc;
        p->kind = (unsigned char)func_ov004_020d3cfc(p->w9d4);
        if (p->kind != 0 && !p->aa8_b4) {
            p->mode |= 4;
        }
    }
    if ((p->mode & 2) && p->kind == 0) {
        p->w9d4 = p->w9d0;
        p->kind = (unsigned char)func_ov004_020d3cfc(p->w9d4);
        if (p->kind != 0 && !p->aa8_b4) {
            p->mode |= 4;
        }
    }
    if (p->mode & 4) {
        p->wf4 = func_ov004_020d3d5c(p->w9d4);
        a = func_ov004_020d3ce4(p->w9d4);
        b = func_ov004_020d3d14(p->w9d4);
        p->wf8 = ((unsigned long long)a << 24) / ((unsigned long long)p->wf4 * b);
        p->wf8 = p->wf8 + 1;
        p->w44 = p->wf8 * (p->wdc + 1);
        if (p->kind == 1) {
            p->w38 = func_ov004_020d6b58(p->w44 * p->wf4 * 2);
            func_ov004_020d57b0(p->w38, p->w44 * p->wf4 * 2);
        } else {
            p->w38 = func_ov004_020d6b58(p->w44 * p->wf4 * 2);
            p->w3c = func_ov004_020d6b58(p->w44 * p->wf4 * 2);
            func_ov004_020d57b0(p->w38, p->w44 * p->wf4 * 2);
            func_ov004_020d57b0(p->w3c, p->w44 * p->wf4 * 2);
        }
        func_020747e4();
    }
    data_020bbb58 = 0;
    p->w40 = 0;
    p->w30 = 0;
    p->w34 = 0;
    p->w48 = 0;
    p->aa9_b7 = 1;
    p->wa70 = 0;
    func_020754f4(p->alarm);
    tick = func_020753bc();
    func_02075698(p->alarm, tick + 5, (unsigned int)(((unsigned long long)33514 * ((1000000ULL << 24) / func_ov004_020d3d14(p->w9d4))) / 64 / 1000), func_ov004_020d6bf8, 0);
    data_020bbb54 = 0;
    p->wa6c = 0;
    if (p->first_a) {
        if (*p->wa78 > 0) {
            func_ov004_020d3dec(p->w9cc, *p->wa78);
            p->we4 = func_ov004_020d3d8c(p->w9cc, *p->wa78);
            p->wa7c = *p->wa78;
        }
    }
    if (p->second_a) {
        if (*p->wa8c > 0) {
            func_ov004_020d3dec(p->w9d0, *p->wa8c);
            p->we8 = func_ov004_020d3d8c(p->w9d0, *p->wa8c);
            p->wa90 = *p->wa8c;
        }
    }
    return 1;
}
}
