struct Obj0204ddac {
    char pad0[0xc];
    unsigned char state;
    char pad1[0x1b];
    int x;
    int y;
    char pad2[0x84];
    unsigned char flip;
    char pad3[7];
    int angle;
};

extern void *func_02008444(void);
extern int func_0202c394(void *a);
extern int func_02010b54(struct Obj0204ddac *p);
extern void *func_0202970c(void);
extern int func_02029708(void *a);
extern int func_0200f5c8(void *a);
extern int func_0200f5e8(void *a);
extern void func_0204deb0(int a);
extern void func_0204debc(void);
extern void func_0204dec8(int a, int b, int c);
extern void func_0204ded8(int a, int b, int c);
extern void func_020571e4(struct Obj0204ddac *p);
extern void func_0204dee8(int a);
extern void func_0204def4(int a, int b);
extern void func_0204df14(int a, int b, int c);
extern void func_0204df30(void);

void func_0204ddac(struct Obj0204ddac *p)
{
    int z;
    int dx;
    int dy;
    short turn;

    if (func_0202c394(func_02008444()) != 0) {
        return;
    }
    if (p->state != 3) {
        return;
    }
    if (func_02010b54(p) == 0) {
        return;
    }
    if (p->angle == 0) {
        return;
    }
    z = func_02029708(func_0202970c());
    func_0204deb0(0);
    dx = p->x - func_0200f5c8(func_02008444()) - 0x80000;
    dy = -p->y + func_0200f5e8(func_02008444()) + 0x60000;
    func_0204debc();
    func_0204dec8(dx >> 7, (dy / 3) >> 5, -(z - 0x600));
    func_0204ded8(0x1000, 0x1555, 0x1000);
    func_020571e4(p);
    func_0204deb0(2);
    func_0204dee8(0);
    func_0204def4(0, 0);
    if (p->flip) {
        turn = p->angle >> 7;
    } else {
        turn = -(p->angle >> 7);
    }
    func_0204df14(turn, 0, 0);
    func_0204def4(0x10000, 0);
    func_0204df14(0, 0xd00, 0);
    func_0204def4(0x10000, 0x3e000);
    func_0204df14(0, 0, 0);
    func_0204df30();
}
