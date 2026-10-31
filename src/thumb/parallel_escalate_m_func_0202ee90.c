extern void func_0202ec8c(unsigned char *object);
extern void func_0202eda4(unsigned char *a, unsigned char *b);
extern int func_0202eca8(unsigned char *p);
extern unsigned int func_0202edd0(unsigned char *p);
extern int func_0202ef48(int a, int b);
extern int func_0206ef28(int a, int b);
extern void func_0202ec94(unsigned char *a, unsigned char *b);
extern void func_0202ecf8(unsigned char *a, int v);

void func_0202ee90(unsigned char *dst, unsigned char *a, unsigned char *b, int op)
{
    int va;
    unsigned int ta;
    int vb;
    unsigned int tb;
    int result;

    func_0202ec8c(dst);
    func_0202eda4(a, b);
    va = func_0202eca8(a);
    ta = func_0202edd0(a);
    vb = func_0202eca8(b);
    tb = func_0202edd0(b);
    result = 0;
    switch (op) {
    case 11:
        result = vb - va;
        break;
    case 12:
        if (ta == 1 && tb == 1) {
            result = vb * va;
        } else {
            result = func_0202ef48(vb, va);
        }
        break;
    case 13:
        if (ta == 1 && tb == 1) {
            result = vb / va;
        } else {
            result = func_0206ef28(vb, va);
        }
        break;
    case 0x3f:
        result = vb % va;
        break;
    case 10:
    case 0x47:
        result = vb + va;
        break;
    }
    func_0202ec94(dst, b);
    func_0202ecf8(dst, result);
}
