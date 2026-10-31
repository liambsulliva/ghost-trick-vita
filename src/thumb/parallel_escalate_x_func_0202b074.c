extern void func_0202b164(int *v);
extern int func_0202b168(int *v);
extern int func_02008444(void);
extern int func_0200f5c8(int camera);
extern int func_0200f5e8(int camera);
extern int func_0206f28c(int y, int x);

void func_0202b074(char *object, int *x, int *y, int third, int fourth)
{
    int s[4];
    int d1;
    int len;
    int r;
    int a;
    unsigned int t;
    unsigned int u;

    func_0202b164(&s[2]);
    func_0202b164(&s[0]);
    s[2] = third - (func_0200f5c8(func_02008444()) + 0x80000);
    s[3] = fourth - (func_0200f5e8(func_02008444()) + 0x60000);
    d1 = func_0202b168(&s[2]) >> 12;
    s[0] = s[2];
    s[1] = (fourth - func_0200f5e8(func_02008444())) * 2 - 0x60000;
    len = func_0202b168(&s[0]) >> 12;
    if (len > 0) {
        r = s[2] / len;
        if (r < 0) {
            r = -r;
        }
    } else {
        r = 0x1000;
    }
    a = func_0206f28c(s[2], r) >> 12;
    *y = a;
    if (a < -0x120) {
        *y = -0x40;
    } else if (a < -0x20) {
        *y = (a >> 2) + 8;
    } else if (a < 0x20) {
        *y = 0;
    } else if (a < 0x120) {
        *y = (a >> 2) - 8;
    } else {
        *y = 0x3f;
    }
    u = 0x140 - d1;
    t = (0x800 - d1) >> 5;
    if (d1 <= 0x40) {
        *x = 0x100;
    } else if (d1 <= 0x140) {
        if (u > t) {
            *x = u;
        } else {
            *x = t;
        }
    } else if (d1 <= 0x800) {
        *x = t;
    } else {
        *x = 0;
    }
}
