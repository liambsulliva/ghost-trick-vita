class Codec {
public:
    virtual void s0();
    virtual void s1();
    virtual int s2(unsigned int len, char *name);
    virtual void s3();
    virtual void s4(void *a);
};

typedef struct {
    int x;
    int y;
    int z;
} Vec;

struct Iter {
    int data[4];
};

struct Target {
    int key;
    int part;
    Vec pos;
    void *owner;
};

struct Scan {
    unsigned char pad0[0x7f];
    unsigned char level;
    unsigned char pad80[0x50];
    Vec *levels;
    unsigned char padd4[0x18];
    unsigned short count;
    unsigned char padee[2];
    struct Target *targets;
};

extern "C" {
extern char data_020cf87c[];
extern char data_0209dc18[];
extern void func_0202fbb0(struct Iter *it);
extern void *func_0200eab4(struct Iter *it);
extern void *func_0200eab8(struct Iter *it);
extern void func_0200eabc(struct Iter *it, int a);
extern int func_0200eac4(struct Iter *it);
extern int func_02008828(void *a, char *b);
extern unsigned int func_020266f4(void *obj);
extern int func_020266dc(void *obj, unsigned int k);
extern void *func_02008444(void);
extern int func_02010194(void *a);
extern void *func_02007bd8(void);
extern Codec *func_02007b70(void *a);
extern void func_02058a80(Vec *out, int e);
extern void func_020083c0(Vec *dst, Vec *a, Vec *b);
extern int func_020452e8(Vec *v);
extern void func_020083b0(Vec *dst, Vec *src);
int func_02061448(struct Scan *p);
}

int func_02061448(struct Scan *p)
{
    struct Iter first;
    struct Iter second;
    Vec v;
    Vec w;
    int found;
    void *obj;
    unsigned int n;
    unsigned int k;
    int e;
    int i;
    int key;

    found = 0;
    p->count = 0;
    func_0202fbb0(&first);
    while (func_0200eac4(&first) != 0) {
        if (func_02008828(func_0200eab4(&first), data_020cf87c) != 0) {
            obj = func_0200eab8(&first);
            n = func_020266f4(obj);
            for (k = 0; k < n; k++) {
                e = func_020266dc(obj, k);
                if (e != 0) {
                    p->count++;
                    if (e == func_02010194(func_02008444())) {
                        found = e;
                    }
                }
            }
        }
        func_0200eabc(&first, 0);
    }
    if (p->count == 0) {
        return 0;
    }
    if (p->targets != 0 && func_02007b70(func_02007bd8()) != 0) {
        func_02007b70(func_02007bd8())->s4(p->targets);
        p->targets = 0;
    }
    p->targets = (struct Target *)func_02007b70(func_02007bd8())->s2(p->count * 0x18, data_0209dc18);
    i = 0;
    func_0202fbb0(&second);
    while (func_0200eac4(&second) != 0) {
        if (func_02008828(func_0200eab4(&second), data_020cf87c) != 0) {
            obj = func_0200eab8(&second);
            n = func_020266f4(obj);
            for (k = 0; k < n; k++) {
                e = func_020266dc(obj, k);
                if (e != 0) {
                    func_02058a80(&v, e);
                    v.z = 0;
                    func_020083c0(&w, &p->levels[p->level - 1], &v);
                    key = func_020452e8(&w);
                    p->targets[i].key = key;
                    p->targets[i].part = e;
                    p->targets[i].owner = obj;
                    func_020083b0(&p->targets[i].pos, &v);
                    i++;
                }
            }
        }
        func_0200eabc(&second, 0);
    }
    return found;
}
