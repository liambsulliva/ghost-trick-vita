extern int func_0200acc8(void *p);
extern void func_0200ad10(void *a, unsigned short b);
extern int func_0200ba2c(void *p);
extern int func_0200ba44(void *p);
extern void *func_0200bbe8(void *p);
extern void func_0202f1bc(void *a, void *b, int c, int d, int e);
extern void func_02038d0c(void *p);
extern int func_0203a2dc(void *p);
extern void *func_02009c1c(void);

void func_0203a27c(unsigned char *param_1)
{
    void *res;
    void *mgr;
    unsigned char *self;
    int v;
    int h;

    self = param_1;
    if (func_0203a2dc(self) == 0x800a) {
        v = func_0200acc8(self + 0xd4);
        res = func_0200bbe8(self + 0x28);
        func_0200ad10(res, (unsigned short)v);
        mgr = func_02009c1c();
        v = func_0200ba2c(self + 0x28);
        h = func_0200ba44(self + 0x28);
        func_0202f1bc(mgr, res, v << 5, h, 4);
    }
    func_02038d0c(self);
}
