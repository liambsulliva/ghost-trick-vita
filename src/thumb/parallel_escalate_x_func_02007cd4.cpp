typedef struct {
    int x;
    int y;
    int z;
} Vec;

class Owner {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
};

class Node {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20(int a, int b);
};

struct Script {
    int w00;
    unsigned short *code;
    int pc;
    unsigned short w0c;
    unsigned short we;
    unsigned short w10;
    unsigned char b0 : 2;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 2;
    unsigned char shift : 2;
    unsigned char w13;
    Owner *owner;
};

extern "C" {
extern Vec data_020a46c8;
extern char data_020cf898[];
extern char data_020cf8b4[];
extern char data_02083a28[];
extern void func_020082d8(Vec *dst, Vec *src);
extern int func_020082e8(void *p);
extern Node *func_020082f8(Owner *owner, char *name);
extern int func_02008350(Node *node);
extern void func_02008378(Vec *dst, Vec *a, Vec *b);
extern Vec *func_020083a0(Node *node);
extern void func_020083a4(Node *node, Vec *v);
extern void func_020083c0(Vec *dst, Vec *a, Vec *b);
extern void func_020083e0(Owner *owner, int v);
extern void func_020083fc(Owner *owner, int v);
extern int func_02008404(int v);
extern void func_02008418(Owner *owner);
extern int func_02008424(void *p);
extern int func_0200842c(unsigned short *p);
extern void *func_02008444(void);
extern void *func_0200847c(void);
extern void func_020084a0(struct Script *p, unsigned short *rec);
extern void func_020084c8(struct Script *p, unsigned short *rec);
extern void func_020084f0(struct Script *p, unsigned short *rec);
extern void func_0200853c(struct Script *p, int v);
extern void func_02008734(struct Script *p, unsigned short *rec);
extern void func_0200da04(void *a, void *b, void *c, char *d);
extern void *func_02027c28(int id);
extern void func_0202a0c0(void *sys, int id);
extern void *func_0202a114(void *sys, int id, int x, int y);
extern void func_02052ad8(int id, int x, int y, int sx, int sy, void *res, Node *node, int s, int t, int a, int b, int c, int d);
extern void func_02059170(Node *node, int a, int b, int c);
extern void func_02059204(Node *node, int a, int x, int y);
extern void func_0205958c(Vec *dst, Node *node, int k);
extern void func_02061c10(void *item, Node *node, int k);
int func_02007cd4(struct Script *p, int arg1, int arg2);
}

static inline void GetPoint(Vec *out, Node *node, int k, Vec *base)
{
    Vec tmp;

    func_0205958c(&tmp, node, k);
    func_02008378(out, base, &tmp);
}

static inline void SetPoint3(Node *node, Vec *a, Vec *b, Vec *c)
{
    Vec s;
    Vec r;

    func_02008378(&s, a, b);
    func_020083c0(&r, &s, c);
    func_020083a4(node, &r);
}

static inline void SetPoint2(Node *node, Vec *a, Vec *b)
{
    Vec r;

    func_020083c0(&r, a, b);
    func_020083a4(node, &r);
}

int func_02007cd4(struct Script *p, int arg1, int arg2)
{
    Vec A;
    Vec B;
    Vec v150;
    Vec v144;
    Vec v138;
    Vec v12c;
    Vec v120;
    Vec v114;
    Vec v108;
    Vec vfc;
    Vec vf0;
    Vec ve4;
    int flag;
    unsigned short *saved;
    Node *n8;
    int ux;
    int uy;
    int a;
    short t;
    int c;
    short clamp;
    int idx;
    int b;
    Node *node;
    short s;
    unsigned short *rec;
    unsigned short op;
    void *item;

    func_020082d8(&A, &data_020a46c8);
    func_020082d8(&B, &data_020a46c8);
    flag = 0;
    idx = 8;
    saved = 0;
    if (p->pc == 0 && arg2 >= 0) {
        rec = &p->code[p->pc];
        for (;;) {
            switch (*rec) {
            case 0xfff6:
                rec += 2;
                break;
            case 0xfff7:
                rec += 3;
                break;
            case 0xfff8:
                rec += 11;
                break;
            case 0xfff9:
                rec += 4;
                break;
            case 0xfffc:
                rec += 5;
                break;
            case 0xfffd:
                rec += 3;
                break;
            case 0xfffa:
                rec += 2;
                break;
            case 0xfff3:
                rec += 2;
                break;
            case 0xfff4:
                if (arg1 != 0) {
                    saved = rec;
                } else {
                    func_020084a0(p, rec);
                }
                rec += 3;
                break;
            case 0xfff5:
                func_020084c8(p, rec);
                rec += 4;
                break;
            case 0xfff0:
                func_020084f0(p, rec);
                rec += 4;
                break;
            case 0xfffe:
            case 0xffff:
                goto scan_done;
            default:
                if (arg2 == (*rec & 0x3ff)) {
                    goto scan_done;
                }
                rec++;
                idx = rec - p->code;
                break;
            }
        }
    }
scan_done:
    for (;;) {
        rec = &p->code[p->pc];
        op = p->code[p->pc];
        switch (op) {
        default:
            p->we = op & 0x3ff;
            p->w10 = (op & 0xfc00) >> 10;
            if (p->shift != 0 && p->w10 > 1) {
                p->w10 >>= p->shift;
                if (p->w10 == 0) {
                    p->w10 = 1;
                }
            }
            if (p->w10 != 0) {
                p->w10 = p->w10 - 1;
                func_0200853c(p, p->we);
                if (func_020082e8(func_02008444()) != 0) {
                    if (rec[1] == 0xfffe || (p->b3 != 0 && p->w13 <= 1 && rec[1] == 0xffff)) {
                        if (p->w10 != 0) {
                            p->w10 = p->w10 - 1;
                        }
                    }
                }
                return 1;
            }
            p->pc += 1;
            break;
        case 0xfffb:
            p->we = rec[1];
            p->w10 = rec[2];
            if (p->shift != 0 && p->w10 > 1) {
                p->w10 >>= p->shift;
                if (p->w10 == 0) {
                    p->w10 = 1;
                }
            }
            if (p->w10 != 0) {
                func_0200853c(p, p->we);
                p->pc = p->pc + 2;
                return 1;
            }
            p->pc += 3;
            break;
        case 0xfffc:
            B.x = (short)rec[3] << 12;
            B.y = (short)rec[4] << 12;
            if (arg1 != 0) {
                arg1 = 0;
                A.x = (short)rec[1] << 12;
                A.y = (short)rec[2] << 12;
                node = func_020082f8(p->owner, data_020cf8b4);
                if (node != 0) {
                    if (func_02008350(node) != 0) {
                        GetPoint(&v150, node, 0, func_020083a0(node));
                        A.x = -A.x;
                        SetPoint2(node, &v150, &A);
                        func_02059204(node, 0, A.x, A.y);
                    } else {
                        func_0205958c(&v144, node, 0);
                        func_020082d8(&v138, func_020083a0(node));
                        SetPoint3(node, &v138, &v144, &A);
                    }
                }
            }
            p->pc = idx;
            if (saved != 0) {
                func_020084a0(p, saved);
                saved = 0;
            }
            break;
        case 0xfffd:
            func_02008734(p, rec);
            p->pc += 3;
            break;
        case 0xfffa:
            func_020083e0(p->owner, rec[1]);
            p->pc += 2;
            break;
        case 0xfff3:
            func_020083fc(p->owner, rec[1]);
            p->pc += 2;
            break;
        case 0xfffe:
            return 1;
        case 0xffff:
            p->pc = 3;
            flag = 1;
            break;
        case 0xfff4:
            node = func_020082f8(p->owner, data_020cf898);
            if (node != 0) {
                func_02059204(node, 0, (short)rec[1] << 12, (short)rec[2] << 12);
                if (flag != 0) {
                    flag = 0;
                    func_0205958c(&v12c, node, 0);
                    func_020082d8(&v120, func_020083a0(node));
                    func_02008378(&v114, &v120, &v12c);
                    if (func_02008404(v114.x) > 0x2710000) {
                        p->owner->slot11();
                        func_02008418(p->owner);
                        return 0;
                    }
                    if (func_02008350(node) != 0) {
                        v120.x = v120.x - B.x - v12c.x;
                        v120.y = v120.y + B.y - v12c.y;
                        func_020083a4(node, &v120);
                    } else {
                        SetPoint3(node, &v120, &B, &v12c);
                    }
                }
            }
            p->pc += 3;
            break;
        case 0xfff5:
            node = func_020082f8(p->owner, data_020cf898);
            if (node != 0) {
                func_02059204(node, rec[1], (short)rec[2] << 12, (short)rec[3] << 12);
                if (rec[1] == 0 && flag != 0) {
                    flag = 0;
                    func_0205958c(&v108, node, 0);
                    func_020082d8(&vfc, func_020083a0(node));
                    if (func_02008350(node) != 0) {
                        vfc.x = vfc.x - B.x - v108.x;
                        vfc.y = vfc.y + B.y - v108.y;
                        func_020083a4(node, &vfc);
                    } else {
                        SetPoint3(node, &vfc, &B, &v108);
                    }
                }
            }
            p->pc += 4;
            break;
        case 0xfff0:
            node = func_020082f8(p->owner, data_020cf898);
            if (node != 0) {
                func_02059170(node, rec[1], rec[2], rec[3]);
            }
            p->pc += 4;
            break;
        case 0xfff6:
            node = func_020082f8(p->owner, data_020cf898);
            if (node != 0) {
                node->slot20((rec[1] >> 12) & 0xf, rec[1] & 0xfff);
            }
            p->pc += 2;
            break;
        case 0xfff7:
            if (p->b2 == 0) {
                if (func_02008424(func_02008444()) == 0) {
                    node = func_020082f8(p->owner, data_020cf898);
                    if (node != 0) {
                        GetPoint(&vf0, node, rec[2], func_020083a0(node));
                        item = func_0202a114(func_0200847c(), rec[1], vf0.x, vf0.y);
                        if (item != 0) {
                            func_02061c10(item, node, rec[2]);
                        }
                    } else {
                        func_0202a0c0(func_0200847c(), rec[1]);
                    }
                }
            }
            p->pc += 3;
            break;
        case 0xfff8:
            if (p->b2 == 0) {
                if (func_02008424(func_02008444()) == 0) {
                    n8 = func_020082f8(p->owner, data_020cf898);
                    if (n8 != 0) {
                        ux = func_0200842c(rec + 2);
                        uy = func_0200842c(rec + 4);
                        a = rec[7];
                        b = rec[8];
                        s = (short)rec[9];
                        t = (short)rec[10];
                        c = rec[1];
                        clamp = s;
                        if (s < 0) {
                            clamp = 0;
                        }
                        GetPoint(&ve4, n8, clamp, func_020083a0(n8));
                        if (func_02008350(n8) != 0) {
                            ve4.x = ve4.x - ux;
                        } else {
                            ve4.x = ve4.x + ux;
                        }
                        ve4.y = ve4.y + uy;
                        if (s >= 0) {
                            ve4.x = ux;
                            ve4.y = uy;
                        }
                        if (func_02008350(n8) != 0 && s < 0) {
                            if (b == 0) {
                                b = 1;
                            } else {
                                b = 0;
                            }
                        }
                        func_02052ad8(c, ve4.x, ve4.y, 0x1000, 0x1000, func_02027c28(rec[6]), n8, s, t, a, b, 0x1f, -1);
                    }
                }
            }
            p->pc += 11;
            break;
        case 0xfff9:
            func_0200da04(func_02027c28(rec[1]), func_02027c28(rec[2]), func_02027c28(rec[3]), data_02083a28);
            p->pc += 4;
            break;
        }
    }
}
