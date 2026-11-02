typedef struct Mtx0205dc54 {
    int m[12];
} Mtx0205dc54;

typedef struct Info0205dc54 {
    unsigned char f0;
    unsigned char f1;
} Info0205dc54;

typedef struct Fade0205dc54 {
    char pad[0x48];
    int f48;
    int f4c;
    int f50;
    unsigned char f54;
    char pad55;
    unsigned short f56;
} Fade0205dc54;

extern Info0205dc54 data_020cff60;
extern void *func_0200eb78(void);
extern int func_02022ca4(void *owner, int value);
extern void *func_0202970c(void);
extern int func_02029708(void *object);
extern void func_0205c9b4(Mtx0205dc54 *mtx);
extern void func_0206e9d4(const Mtx0205dc54 *src, Mtx0205dc54 *dst, int x, int y, int z);
extern void func_0206ea80(const Mtx0205dc54 *src, Mtx0205dc54 *dst, int x, int y, int z);
extern void func_0206ead0(const Mtx0205dc54 *a, const Mtx0205dc54 *b, Mtx0205dc54 *ab);
extern void func_0205c9bc(int a, int b, int c, int d, int e, int f, int g, int h);
extern int func_0205d834(int value);
extern int func_0205d820(int value);
extern void func_0205ddf4(Mtx0205dc54 *mtx, int sin, int cos);
extern void func_0206f77c(Mtx0205dc54 *mtx);
extern void func_0205c9f8(int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e, unsigned int f);
extern void func_0205ca28(int value);
extern void func_0205ca1c(int value);
extern void func_0205ddfc(int s, int t);
extern void func_0205ca34(int x, int y, int z);
extern void func_0205ca50(void);

void func_0205dc54(Fade0205dc54 *self)
{
    Mtx0205dc54 base;
    Mtx0205dc54 rot;
    Mtx0205dc54 result;
    Mtx0205dc54 scale;
    int sin;
    int tex;
    unsigned short angle;
    unsigned int i;
    int alpha;
    int s;

    if (self->f54 != 0) {
        return;
    }
    tex = func_02022ca4(func_0200eb78(), self->f56);
    if (self->f50 < (data_020cff60.f1 + 1) * 3 + 10 || self->f48 < 0) {
        return;
    }
    func_0205c9b4(&base);
    func_0206e9d4(&base, &base, 0, 0, -(func_02029708(func_0202970c()) - 0x480));
    func_0206ea80(&base, &base, 0x1000, 0x1555, 0x1000);
    func_0205c9bc(7, 1, 5, 5, 3, 0, 1, 0x20000);
    for (i = 0; i < 4; i++) {
        alpha = self->f48 >> 2;
        if (i != 0 && alpha > 8) {
            alpha = 8;
        }
        func_0205c9b4(&scale);
        s = (self->f4c >> 1) * (i + 1) << 5;
        func_0206ea80(&scale, &scale, s + 0x1000, s + 0x1000, 0x1000);
        func_0206ead0(&scale, &base, &scale);
        angle = (i + 1) * (self->f4c * -100 >> 2);
        sin = func_0205d834(angle);
        func_0205ddf4(&rot, sin, func_0205d820(angle));
        func_0206ead0(&rot, &scale, &result);
        func_0206f77c(&result);
        func_0205c9f8(0, 0, 3, i + 0x34, alpha, 0);
        func_0205ca28(2);
        func_0205ca1c(tex);
        func_0205ddfc(0, 0);
        func_0205ca34(-0x1000, 0xc00, 0);
        func_0205ddfc(0, 0xc0000);
        func_0205ca34(-0x1000, -0xc00, 0);
        func_0205ddfc(0x100000, 0);
        func_0205ca34(0x1000, 0xc00, 0);
        func_0205ddfc(0x100000, 0xc0000);
        func_0205ca34(0x1000, -0xc00, 0);
        func_0205ca50();
    }
}
