struct Vec {
    int x;
    int y;
    int z;
};

class Sprite {
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
    virtual void slot35(signed char alpha);
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
    virtual void slot46(int sx, int sy);
};

class Heap {
public:
    virtual void slot0();
    virtual void slot1();
    virtual Vec *slot2(int size, void *tag);
};

struct Trail {
    unsigned char pad0[0x28];
    int x;
    int y;
    unsigned char pad30[0x8c];
    int handle;
    int res;
    Sprite *objA;
    Sprite *objB;
    Sprite *slots[15];
    Vec *trail;
    unsigned char pad10c[0xc];
    short ax;
    short ay;
    short bx;
    short by;
    unsigned char active;
    unsigned char side;
    unsigned char flip;
    unsigned char pad123;
    int pos;
    int pos2;
    int fade;
    int fade2;
};

extern "C" {
extern char data_0209b874;
extern int data_020a8b0c;
extern int data_020a8b10;
extern int data_020a8b14;
extern int data_020a8b18;
extern int data_020a8b1c;
extern int data_020a8b20;
extern int data_020a8b24;
extern int data_020a8b28;
extern int data_020a8b2c;
extern int data_020a8b30;
extern int data_020a8b34;
extern int data_020a8b38;
extern int data_020a8b3c;
extern int data_020a8b40;
extern int data_020a8b44;
extern int data_020a8b48;
extern int data_020a8b4c;
extern int data_020a8b50;
extern int data_020a8b54;
extern int data_020a8b58;
extern int data_020a8b5c;
extern int data_020a8b60;
extern int data_020a8b64;
extern int data_020a8b68;
extern int data_020a8b6c;
extern int data_020a8b70;
extern int data_020a8b74;
extern int data_020a8b78;
extern int data_020a8b7c;
extern int data_020a8b80;
extern int data_020a8b84;
extern int data_020a8b88;
extern int data_020a8b8c;
extern int data_020a8b90;
extern int data_020a8b94;
extern int data_020a8b98;
extern int data_020a8b9c;
extern int data_020a8ba0;
extern int data_020a8ba4;
extern int data_020a8ba8;
extern int data_020a8bac;
extern int data_020a8bb0;
extern int data_020a8bb4;

void *func_02008444(void);
int func_02031af4(void *system, int handle);
void func_02031b04(void *system, int *handle);
void func_0202f64c(int *res);
void *func_02007bd8(void);
Heap *func_02007b70(void *system);
void func_0204a1b8(Trail *p, Sprite **slot, int hidden);
int func_0202c394(void *object);
int func_0201132c(void *game);
int func_0204a150(Trail *p, int *pos, int *range, int *start, int *from, int *to, int *def);
void func_0200ee34(Sprite *obj, unsigned short r, unsigned short g, unsigned short b);
void func_0204a184(Trail *p, int *pos, int *out, int *range, int *start, int *from, int *to);
void func_0204a1fc(Trail *p, Sprite *obj, int id, int flag);
int func_0200f5c8(void *object);
int func_0200f5e8(void *object);
int func_02010b54(Sprite *obj);
Vec *func_020083b0(Vec *dst, Vec *src);

void func_0204a228(Trail *p)
{
    int dx;
    int dy;
    int h;
    int xr;
    int sx;
    int sy;
    int handle;
    unsigned int i;
    int j;
    int a;
    int b;
    int c;
    int d;
    int x;
    int y;
    int yb;
    int flag;
    int step;
    int lim;

    handle = p->handle;
    if (handle >= 0 && func_02031af4(func_02008444(), handle) == 0 && p->res != 0) {
        func_02031b04(func_02008444(), &p->handle);
        func_0202f64c(&p->res);
    }
    if (p->trail == 0) {
        p->trail = func_02007b70(func_02007bd8())->slot2(0xb4, &data_0209b874);
    }
    func_0204a1b8(p, &p->objA, 1);
    func_0204a1b8(p, &p->objB, 1);
    for (i = 0; i < 15; i++) {
        func_0204a1b8(p, &p->slots[i], 0);
    }
    if (func_0202c394(func_02008444()) != 0) {
        if (func_0201132c(func_02008444()) != 0) {
            a = func_0204a150(p, &p->pos, &data_020a8bb0, &data_020a8ba4, &data_020a8ba8, &data_020a8b74, &data_020a8b60);
            b = func_0204a150(p, &p->pos, &data_020a8b44, &data_020a8b48, &data_020a8b4c, &data_020a8b28, &data_020a8b24);
            c = func_0204a150(p, &p->pos, &data_020a8b88, &data_020a8b50, &data_020a8b54, &data_020a8b2c, &data_020a8b98);
        } else {
            a = func_0204a150(p, &p->pos, &data_020a8b58, &data_020a8ba0, &data_020a8b5c, &data_020a8b30, &data_020a8b18);
            b = func_0204a150(p, &p->pos, &data_020a8b14, &data_020a8b64, &data_020a8b9c, &data_020a8b0c, &data_020a8b7c);
            c = func_0204a150(p, &p->pos, &data_020a8b80, &data_020a8b34, &data_020a8b90, &data_020a8b94, &data_020a8b8c);
        }
        func_0200ee34(p->objA, a, b, c);
        func_0200ee34(p->objB, a, b, c);
        d = func_0204a150(p, &p->fade, &data_020a8b84, &data_020a8b70, &data_020a8b1c, &data_020a8b3c, &data_020a8b10);
        p->objA->slot35(d);
        p->objB->slot35(d);
        func_0204a184(p, &p->pos, &p->pos2, &data_020a8bac, &data_020a8b38, &data_020a8b68, &data_020a8b6c);
        func_0204a184(p, &p->fade, &p->fade2, &data_020a8b20, &data_020a8bb4, &data_020a8b40, &data_020a8b78);
        func_0204a1fc(p, p->objA, 0xa90, 1);
        func_0204a1fc(p, p->objB, 0xa90, 1);
        for (i = 0; i < 15; i++) {
            func_0204a1fc(p, p->slots[i], 0xa88, 0);
        }
        x = p->x - func_0200f5c8(func_02008444()) - 0x80000;
        y = -p->y + func_0200f5e8(func_02008444()) + 0x60000;
        dx = (p->ax - p->bx) << 12;
        dy = (p->ay - p->by) << 12;
        h = dy;
        xr = x + dx;
        yb = y - dy;
        if (x < -0x84000) {
            x = -0x84000;
            if (p->side == 0) {
                p->objB->slot11();
            }
        }
        if (y > 0x64000) {
            y = 0x64000;
            if (p->side == 1) {
                p->objB->slot11();
            }
            h = 0x64000 - yb;
        }
        if (p->side == 0) {
            p->objB->slot13(x, y, 0);
            p->objB->slot46(0x2000, h);
            if (p->flip != 0) {
                p->objA->slot13(x + 0x2000, yb, 0);
                p->objA->slot46(-0x84000 - x, 0x2000);
            } else {
                p->objA->slot13(x, yb, 0);
                p->objA->slot46(0x84000 - x, 0x2000);
            }
        } else {
            p->objB->slot13(x, y, 0);
            p->objB->slot46(dx, 0x2000);
            if (p->flip != 0) {
                p->objA->slot13(xr, y - 0x2000, 0);
                p->objA->slot46(0x2000, y - 0x64000);
            } else {
                p->objA->slot13(xr, y, 0);
                p->objA->slot46(0x2000, y + 0x64000);
            }
        }
        for (j = 1; j < 15; j++) {
            if (p->active == 1 && func_02010b54(p->slots[j]) == 0) {
                p->slots[j]->slot10();
                break;
            }
        }
        for (j = 1; j < 15; j++) {
            func_020083b0(&p->trail[15 - j], &p->trail[14 - j]);
        }
        if (p->active == 0) {
            if (p->side == 0) {
                if (p->flip != 0) {
                    p->trail[0].x = func_0200f5c8(func_02008444()) - 0x4000;
                } else {
                    p->trail[0].x = func_0200f5c8(func_02008444()) + 0x104000;
                }
                p->trail[0].y = p->y + dy;
            } else {
                p->trail[0].x = p->x + dx;
                if (p->flip != 0) {
                    p->trail[0].y = func_0200f5e8(func_02008444()) - 0x4000;
                } else {
                    p->trail[0].y = func_0200f5e8(func_02008444()) + 0xc4000;
                }
            }
            p->slots[0]->slot10();
            p->active = 1;
        }
        for (j = 0; j < 15; j++) {
            (*(Sprite **)((char *)p + j * 4 + 0xcc))->slot46(0x2000, 0x2000);
            sx = p->trail[j].x - func_0200f5c8(func_02008444()) - 0x80000;
            sy = -p->trail[j].y + func_0200f5e8(func_02008444()) + 0x60000;
            (*(Sprite **)((char *)p + j * 4 + 0xcc))->slot13(sx, sy, 0);
            if (func_0201132c(func_02008444()) != 0) {
                if (j < 7) {
                    func_0200ee34((*(Sprite **)((char *)p + j * 4 + 0xcc)), 0, 0x1f, 0);
                    (*(Sprite **)((char *)p + j * 4 + 0xcc))->slot35((j + 1) * 4);
                } else if (j > 7) {
                    func_0200ee34((*(Sprite **)((char *)p + j * 4 + 0xcc)), 0, 0x1f, 0);
                    (*(Sprite **)((char *)p + j * 4 + 0xcc))->slot35((15 - j) * 4);
                } else {
                    func_0200ee34((*(Sprite **)((char *)p + j * 4 + 0xcc)), 0, 0x1f, 0);
                    (*(Sprite **)((char *)p + j * 4 + 0xcc))->slot35(0x1f);
                }
            } else {
                if (j < 7) {
                    func_0200ee34((*(Sprite **)((char *)p + j * 4 + 0xcc)), 0x1f, 0, 0);
                    (*(Sprite **)((char *)p + j * 4 + 0xcc))->slot35((j + 1) * 4);
                } else if (j > 7) {
                    func_0200ee34((*(Sprite **)((char *)p + j * 4 + 0xcc)), 0x1f, 0, 0);
                    (*(Sprite **)((char *)p + j * 4 + 0xcc))->slot35((15 - j) * 4);
                } else {
                    func_0200ee34((*(Sprite **)((char *)p + j * 4 + 0xcc)), 0x1f, 0, 0);
                    (*(Sprite **)((char *)p + j * 4 + 0xcc))->slot35(0x1f);
                }
            }
        }
        if (p->side == 0) {
            flag = 0;
            step = -0x2000;
            lim = p->x;
            if (p->flip != 0) {
                step = 0x2000;
                lim = p->x - 0x2000;
            }
            p->trail[0].x += step;
            if (p->trail[0].x < lim && p->flip == 0) {
                flag = 1;
            }
            if (p->trail[0].x > lim && p->flip == 1) {
                flag = 1;
            }
            if (flag != 0) {
                p->trail[0].x = p->x;
                p->trail[0].y -= 0x2000;
                if (p->trail[0].y < p->y + 0x2000) {
                    p->active = 0;
                }
            }
        } else {
            flag = 0;
            step = -0x2000;
            lim = p->y + 0x2000;
            if (p->flip != 0) {
                step = 0x2000;
                lim = p->y;
            }
            p->trail[0].y += step;
            if (p->trail[0].y < lim && p->flip == 0) {
                flag = 1;
            }
            if (p->trail[0].y > lim && p->flip == 1) {
                flag = 1;
            }
            if (flag != 0) {
                p->trail[0].y = p->y;
                p->trail[0].x -= 0x2000;
                if (p->trail[0].x < p->x + 0x2000) {
                    p->active = 0;
                }
            }
        }
    } else {
        p->objA->slot11();
        p->objB->slot11();
        for (i = 0; i < 15; i++) {
            p->slots[i]->slot11();
        }
        p->active = 0;
    }
}
}
