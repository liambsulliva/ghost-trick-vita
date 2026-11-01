class Sink {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3(void *obj, int arg);
};

struct Iter {
    int data[4];
};

extern "C" {
extern char data_020cfe48[];
extern char data_020ce900[];
extern void func_0202fbcc(struct Iter *it, int kind);
extern void *func_0200eab4(struct Iter *it);
extern void *func_0200eab8(struct Iter *it);
extern void func_0200eabc(struct Iter *it, int a);
extern int func_0200eac4(struct Iter *it);
extern int func_02008828(void *a, char *b);
extern int func_02026c10(void *x);
extern char *func_02026e9c(void *obj);
extern int func_02078fbc(char *a, signed char *b);
extern int func_020790c8(signed char *a, char *b);
void func_02026d34(Sink *self, void *x, signed char *pat);
}

void func_02026d34(Sink *self, void *x, signed char *pat)
{
    struct Iter it;
    void *obj;

    func_0202fbcc(&it, 7);
    if (pat[0] == '*') {
        while (func_0200eac4(&it) != 0) {
            if (func_02008828(func_0200eab4(&it), data_020cfe48) != 0) {
                obj = func_0200eab8(&it);
                self->s3(obj, func_02026c10(x));
            } else if (func_02008828(func_0200eab4(&it), data_020ce900) != 0) {
                obj = func_0200eab8(&it);
                self->s3(obj, func_02026c10(x));
            }
            func_0200eabc(&it, 0);
        }
    } else if (pat[0] == '@') {
        while (func_0200eac4(&it) != 0) {
            if (func_02008828(func_0200eab4(&it), data_020cfe48) != 0) {
                obj = func_0200eab8(&it);
                if (func_02078fbc(func_02026e9c(obj), pat + 1) != 0) {
                    self->s3(obj, func_02026c10(x));
                }
            } else if (func_02008828(func_0200eab4(&it), data_020ce900) != 0) {
                obj = func_0200eab8(&it);
                if (func_02078fbc(func_02026e9c(obj), pat + 1) != 0) {
                    self->s3(obj, func_02026c10(x));
                }
            }
            func_0200eabc(&it, 0);
        }
    } else {
        while (func_0200eac4(&it) != 0) {
            if (func_02008828(func_0200eab4(&it), data_020cfe48) != 0) {
                obj = func_0200eab8(&it);
                if (func_020790c8(pat, func_02026e9c(obj)) == 0) {
                    self->s3(obj, func_02026c10(x));
                }
            } else if (func_02008828(func_0200eab4(&it), data_020ce900) != 0) {
                obj = func_0200eab8(&it);
                if (func_020790c8(pat, func_02026e9c(obj)) == 0) {
                    self->s3(obj, func_02026c10(x));
                }
            }
            func_0200eabc(&it, 0);
        }
    }
}
