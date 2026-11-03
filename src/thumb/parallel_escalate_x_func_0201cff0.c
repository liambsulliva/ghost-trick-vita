struct Field;
struct Obj;

struct ObjVtbl {
    char pad0[0x1c];
    int (*get)(struct Obj *obj, unsigned int id);
};

struct Obj {
    struct ObjVtbl *vtbl;
};

struct Pair8 {
    char first[8];
    char second[8];
};

struct Pair16 {
    char first[16];
    char second[16];
};

struct Board {
    struct Obj *obj;
    char pad4[0x34];
    struct Field *field;
    char pad3c[4];
    unsigned char ready;
    char pad41[3];
    char sub_a[0x20];
    char sub_b[0x20];
    struct Pair8 small[0x72];
    struct Pair8 mid[0x72];
    struct Pair16 big[0x72];
};

struct Code {
    unsigned char lo;
    unsigned char hi;
};

extern int func_0201cf74(struct Field *field);
extern int func_0201d2f4(struct Board *board, unsigned int x, unsigned int y);
extern void func_0201d3a4(struct Board *board, int index);
extern char *func_0201d240(char *object);
extern char *func_0201d244(char *object);
extern void func_0207303c(const void *src, void *dst, unsigned int size);
extern void func_0207c594(void *object, int size);
extern struct Code func_0200e410(struct Field *field, unsigned int id, int mask);
extern void func_0201cabc(struct Code *last, struct Code *code);
extern int func_0201cfd0(struct Board *board, unsigned int id);
extern void func_0201d248(char *object, signed char value);
extern int func_0201d24c(char *object, signed char index);

int func_0201cff0(struct Board *o, unsigned int a, unsigned int b)
{
    struct Code last;
    struct Code code_a;
    struct Code code_b;
    int mode;
    int want;
    int n;
    signed char s;
    unsigned int i;

    if (a > 0xff00 || b > 0xff00) {
        return 0;
    }
    if (o->obj == 0 || o->field == 0) {
        return 0;
    }
    mode = func_0201cf74(o->field);
    if (func_0201d2f4(o, a, b) == 1) {
        return 1;
    }
    want = 3;
    if (mode == 2) {
        want = 0;
    }
    if (o->ready == 0) {
        func_0201d3a4(o, 0);
        func_0201d3a4(o, 1);
        func_0201d3a4(o, 2);
        o->ready = 1;
    }
    if (b == 0xff) {
        return 0;
    }
    if (a > 0x72 || b > 0x72) {
        return 1;
    }
    if (mode == 0) {
        func_0207303c(o->small[a].first, func_0201d240(o->sub_a), 8);
        func_0207303c(o->small[b].second, func_0201d240(o->sub_b), 8);
    } else if (mode == 1) {
        func_0207303c(o->mid[a].first, func_0201d240(o->sub_a), 8);
        func_0207303c(o->mid[b].second, func_0201d240(o->sub_b), 8);
    } else if (mode == 2) {
        func_0207303c(o->big[a].first, func_0201d244(o->sub_a), 16);
        func_0207303c(o->big[b].second, func_0201d244(o->sub_b), 16);
    }
    func_0207c594(&last, 2);
    code_a = func_0200e410(o->field, a, 0xfffe);
    func_0201cabc(&last, &code_a);
    n = o->obj->vtbl->get(o->obj, a);
    s = n + func_0201cfd0(o, a);
    func_0201d248(o->sub_a, s);
    code_b = func_0200e410(o->field, b, 0xfffe);
    func_0201cabc(&last, &code_b);
    n = o->obj->vtbl->get(o->obj, b);
    s = n + func_0201cfd0(o, b);
    func_0201d248(o->sub_b, s);
    for (i = 0; i < 0x20; i++) {
        if (func_0201d24c(o->sub_a, i) != 0 && func_0201d24c(o->sub_b, i) != 0) {
            if (mode == 2) {
                return 1;
            }
            if (want == func_0201d24c(o->sub_a, i) || want == func_0201d24c(o->sub_b, i)) {
                return 1;
            }
        }
        if (want == func_0201d24c(o->sub_a, i) && (want == func_0201d24c(o->sub_b, i + 1) || want == func_0201d24c(o->sub_b, i - 1))) {
            return 1;
        }
    }
    return 0;
}
