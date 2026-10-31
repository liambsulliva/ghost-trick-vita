extern void func_0202bcd4(unsigned char *a, unsigned char *b);
extern void func_0202bcf0(unsigned char *a, unsigned char *b);
extern void func_0206e79c(unsigned char *a, unsigned char *b, unsigned char *c);
extern int func_0206f28c(int a, int b);

void func_0202bc0c(unsigned char *p, unsigned char *q)
{
    if (q == 0) {
        func_0202bcd4(p + 0x4c, p + 0x28);
        func_0202bcf0(p + 0x20, p + 0x18);
        return;
    }
    func_0206e79c(p + 0x28, q + 0x4c, p + 0x4c);
    p[0x20] = ((p[0x18] + 1) * (q[0x20] + 1)) >> 5;
    p[0x21] = ((p[0x19] + 1) * (q[0x21] + 1)) >> 5;
    p[0x22] = ((p[0x1a] + 1) * (q[0x22] + 1)) >> 5;
    if (p[0x20] != 0) {
        p[0x20]--;
    }
    if (p[0x21] != 0) {
        p[0x21]--;
    }
    if (p[0x22] != 0) {
        p[0x22]--;
    }
    *(int *)(p + 0x24) = func_0206f28c(*(int *)(p + 0x1c), *(int *)(q + 0x24)) / 100;
}
