class Obj {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual int s4();
};

class Src {
public:
    virtual Obj *s0();
};

struct Sub {
    int data;
    unsigned int lo : 8;
    unsigned int mode : 8;
    unsigned int mid : 15;
    unsigned int on : 1;
    unsigned char rest[0x20];
};

struct Entry {
    Src *src;
    unsigned int count : 15;
    unsigned int locked : 1;
    unsigned int pad : 16;
    struct Sub subs[1];
};

union Id {
    unsigned int w;
    struct {
        unsigned int flag : 1;
        unsigned int index : 15;
        unsigned int slot : 16;
    } b;
};

struct Mgr {
    int head;
    Obj **objs;
    int size;
    struct Entry **entries;
};

extern "C" {
extern unsigned int func_02021200(void *k);
extern int func_02008828(Obj *a, Src *b);
extern void func_0201ea68(void *k);
extern void func_0201ee68(int a);
extern void func_0201ea28(void *k, unsigned int id);
extern void func_0202152c(struct Mgr *m, struct Entry *e, Obj *obj);
extern void func_020215f8(struct Mgr *m, void *k, struct Sub *s);
extern void func_02021070(struct Mgr *m, void *k, Obj *obj, struct Sub *s);
extern void func_020213a0(struct Mgr *m, void *k, Obj *obj, struct Sub *s);
Obj *func_02021658(struct Mgr *m, void *k, Obj *obj);
}

Obj *func_02021658(struct Mgr *m, void *k, Obj *obj)
{
    union Id id;
    struct Entry *e;
    unsigned int i;
    struct Sub *sub;

    id.w = func_02021200(k);
    if (id.b.index == 0x7fff) {
        return 0;
    }
    if (obj != 0) {
        m->objs[id.b.slot] = obj;
    } else {
        obj = m->objs[id.b.slot];
    }
    e = m->entries[id.b.index];
    if (e->src != 0) {
        if (obj != 0) {
            if (func_02008828(obj, e->src) == 0) {
                func_0201ea68(k);
                func_0201ee68((int)e->src);
                func_0201ee68(obj->s4());
                obj = 0;
            }
        } else {
            obj = e->src->s0();
            m->objs[id.b.slot] = obj;
        }
    } else {
        obj = 0;
    }
    if (obj == 0) {
        if (id.b.flag) {
            func_0201ea28(k, func_02021200(k));
        }
        return 0;
    }
    if (!id.b.flag) {
        return obj;
    }
    if (e->locked == 0) {
        func_0202152c(m, e, obj);
    }
    func_02021200(k);
    for (i = 0; i < e->count; i++) {
        sub = &e->subs[i];
        if (sub->on) {
            func_020215f8(m, k, sub);
        }
        if (sub->mode & 0x80) {
            func_02021070(m, k, obj, sub);
        } else {
            func_020213a0(m, k, obj, sub);
        }
    }
    return obj;
}
