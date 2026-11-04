class Script;

struct Vec {
    int x;
    int y;
    int z;
};

struct Doc {
    int f0;
    int f4;
};

struct Iter {
    int f0;
    int f4;
    int f8;
    int fc;
};

struct Ids {
    unsigned short id[3];
};

struct Info {
    unsigned char pad[0x12];
    short f12;
};

struct Clock {
    unsigned char b0;
    unsigned char b1;
};

class Cmd {
public:
    virtual void c0();
    virtual void c1();
    virtual void c2(Script *s, signed char *name);
};

class Actor {
public:
    virtual void a0();
    virtual void a1();
    virtual void a2();
    virtual void a3();
    virtual void a4();
    virtual void a5();
    virtual void a6();
    virtual void a7();
    virtual void a8();
    virtual void a9();
    virtual void a10();
    virtual void a11();
    virtual int a12();
    virtual void a13();
    virtual void a14();
    virtual void a15();
};

class Maker {
public:
    virtual void *make();
};

class Node {
public:
    virtual void n0();
    virtual void n1();
    virtual void n2();
    virtual void n3(Doc *doc);
};

class Panel {
public:
    virtual void p0();
    virtual void p1();
    virtual void p2();
    virtual void p3();
    virtual void p4();
    virtual void p5();
    virtual void p6();
    virtual void p7();
    virtual void p8();
    virtual void p9();
    virtual void p10();
    virtual void p11();
    virtual void p12();
    virtual void p13();
    virtual void p14();
    virtual void p15();
    virtual void p16();
    virtual void p17();
    virtual void p18();
    virtual void p19();
    virtual void p20();
    virtual void p21();
    virtual void p22();
    virtual void p23();
    virtual void p24();
    virtual void p25();
    virtual void p26();
    virtual void p27();
    virtual void p28();
    virtual void p29();
    virtual void p30();
    virtual void p31();
    virtual void p32();
    virtual void p33(int id);
};

class Script {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual int v6();
    virtual int v7();
    virtual int v8();
    virtual int v9();
    virtual int v10();
    virtual int v11();
    virtual int v12();
    virtual int v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17(int a);
    unsigned short *code;
    int pc;
    int wait;
    unsigned char b10;
    void *f14;
    int f18;
};

extern "C" {
extern Ids data_02084fbc;
extern char data_02084fc4[];
extern char data_02084fd0[];
extern char data_02084fe0[];
extern char data_02084fec[];
extern char data_02084ffc[];
extern char data_02085008[];
extern char data_02085014[];
extern char data_02085020[];
extern char data_02085028[];
extern char data_02085030[];
extern char data_02085048[];
extern char data_02085064[];
extern char data_02085078[];
extern char data_02085088[];
extern char data_02085098[];
extern char data_020850b0[];
extern char data_020850c8[];
extern char data_020850cc[];
extern int data_020abf34;
extern int data_020abf38;
extern char data_020bbbb4[];
extern Info data_020c9e88;
extern char data_020cd798[];
extern char data_020cdfac[];
extern char data_020ce1e8[];
extern char data_020cedc4[];
extern char data_020cf75c[];
extern char data_020cf860[];
extern char data_020cf87c[];
extern char data_020cf898[];
extern char data_020cf8b4[];
extern char data_020cf980[];
extern char data_020cfb90[];
extern char data_020cfbd4[];
extern char data_020cfcf0[];
extern char data_020cfd28[];
extern Clock data_020cff60;
unsigned int func_01ff80b8(void *obj);
void func_020082d8(Vec *dst, const Vec *src);
void *func_020082f8(void *obj, char *name);
void func_02008378(Vec *out, const Vec *a, const Vec *b);
const Vec *func_020083a0(void *obj);
void func_02008418(void *obj);
void *func_02008444(void);
void *func_0200847c(void);
int func_02008828(void *obj, char *name);
void *func_02008860(void);
void *func_02008eb0(void);
void *func_02009900(void);
void func_02009bbc(void *game);
void func_02009bcc(void *game, int a, int b);
void func_02009bdc(void *game, int a, int b);
void func_0200c82c(void *sys, int a);
void func_0200e930(void *game, unsigned char a);
void *func_0200eab4(Iter *it);
void *func_0200eab8(Iter *it);
void func_0200eabc(Iter *it, int a);
int func_0200eac4(Iter *it);
void func_0200ec24(void *game, int a);
void func_0200ecb0(void *game, int a, int b);
void func_0200ee34(void *obj, int r, int g, int b);
int func_0200eec4(void *game);
void func_0200eee0(void *game);
void func_0200eef4(void *game);
void func_0200f084(void *game, int a, int b, int c, int d);
int func_0200f178(void *game);
int func_0200f190(void *game);
void func_0200f1a8(void *game, int a);
void func_0200f604(void *game, int x, int y, int a);
void func_0200f634(void *game, int a);
void func_0200f70c(Vec *v);
void func_0200f780(void *game, int a);
void func_0200f918(void *game, int a);
void func_0200f958(void *game, int a);
void func_0200fa64(void *game, int a, int b);
void func_0200faa4(void *game, void *obj, int a, int b, int c, int d);
void *func_0201003c(void);
void func_02010074(void *game, int a);
void func_020100a0(void *game, int a);
void func_020101c0(void *game, void *obj, int a, int b);
void func_020101fc(void *game, int a);
void func_02010264(void *game, int a);
void func_02010350(void *game, signed char *name);
void func_02010384(void *game, int a);
void func_02010394(void *game, void *obj, void *b, short c, int d);
void func_02010440(void *game, void *obj, short a, signed char *b, signed char *c, signed char *d, int e);
void func_020104b4(void *game, void *obj, short a, int b, int c);
void func_0201052c(void *game, void *b, int c, int d, int e);
void func_020105d4(void *game, int a, int b, signed char *c, signed char *d, signed char *e, int f);
void func_0201062c(void *game, int a, int b, int c, int d);
void func_020106a0(void *game);
void func_020106b8(void *game, unsigned char a);
void func_020106d8(void *game, int a);
void func_020106f8(void *game);
void func_02010754(void *game);
void func_02010778(void *game, void *obj, void *b, int c, int d);
void func_02010824(void *game, int a);
void func_02010874(void *game);
void func_02010974(void *game, int a);
void func_02010ba0(void *game, void *obj, int a, int b, int c);
void func_02010c28(void *game, void *obj, int a, int b);
int func_02010c70(void *game, void *obj);
void func_02010ca4(void *game);
void func_02010cdc(void *game);
void func_02010d18(void *game);
void func_02010d30(void *game);
void func_02010da4(void *game, int a);
void func_02010dc4(void *obj);
void func_02011074(void *game);
void func_02011094(void *game);
void func_020110ec(void *game);
void *func_0201123c(void *list, int a);
int func_02011244(void *game);
void *func_02011264(void);
void func_02011288(void *game);
int func_020112a8(void *game);
int func_02011320(void *game);
void func_02011478(void *game, int a);
void func_020114dc(void *game, int a);
void func_020118a8(void *game, void *a);
void func_0201192c(void *game, int a);
void func_02011958(void *game, void *a, int b);
void func_02011a14(void *game, int a);
void func_02011a28(void *game, int a);
void func_02011b04(void *game);
void func_02011cc4(void *game);
void func_02011cd0(void *game);
void func_02011cdc(void *game, int a);
void func_02011d1c(void *game);
void func_02011d74(void *game);
void func_02011da4(void *game, void *a, void *b, int c);
void func_02011e48(void *game, void *a, void *b, int c, unsigned char d);
void func_02011f34(void *game, void *a, int b, signed char *c, signed char *d, signed char *e);
void func_02011fac(void *game, void *a, int b, signed char *c, signed char *d, signed char *e, unsigned char f);
void func_020120b0(void *game, int a, int b, int c);
void func_02012110(void *game, int a);
const Vec *func_0201230c(void *game);
void func_020123a0(void *game, int a);
void func_020125d4(void *game, int a);
void func_0201354c(void *game, int a);
void func_02013a24(void *game, int a);
void func_020141f4(void *game);
void func_020153d8(void *obj, void *owner);
void func_02015418(void *mgr);
void func_02015748(void *mgr);
void func_02015a34(void *mgr);
void func_02015a54(void *game, void *a, int b);
void func_02015b70(void *mgr);
void func_02015b98(void *mgr);
void func_02015bb4(void *mgr);
void func_02015bcc(void *mgr);
void func_020184f4(void *game, int a, int b);
void *func_0201ee30(char *a, char *b);
void *func_0201ee6c(unsigned int a, char *b);
void func_0201f974(void *obj, unsigned char a, unsigned char b, int c);
int func_0201fa5c(void *obj, unsigned char a, unsigned char b);
void func_0201fcd0(void *obj, int a);
void *func_0201ff10(void *game, int a);
void func_02020258(void *obj, int a);
int func_02020ac8(void *obj);
void func_02020c98(Doc *doc);
void func_02020ca8(Doc *doc);
void *func_02020db0(Doc *doc, char *name);
void *func_02022f04(void *obj, int a);
void func_02022f2c(void *obj, void *a, int b);
void func_02022f34(void *obj, char *name, int a);
int func_02022fa4(void *obj, char *name);
int func_02022fc0(void *obj, void *a);
void func_02026534(void *game);
void func_02026544(void *game);
int func_0202657c(void *game);
unsigned int func_020265c4(unsigned short *p);
unsigned int func_020265dc(unsigned short *p);
void func_020265ec(void *game, int a);
int func_020265fc(void *game);
void func_02026608(void *game, int a);
void func_0202661c(void *game);
int func_0202662c(void *game);
void func_0202663c(void *game, signed char *name);
void func_0202665c(void *game);
int func_02026668(Script *s, int a);
int func_02026680(Script *s);
int func_02026698(void *game);
int func_020266ac(void *game);
int func_020266c0(void);
void *func_020266d4(void *a);
void *func_020266dc(void *obj, int a);
void func_020266e8(void *game, void *a);
unsigned int func_020266f4(void *obj);
void func_02026700(void *a, int b);
void func_02026734(void *obj, int a);
void func_02026744(void *obj, int a);
void func_02026770(void *obj, int a, int b, int c);
void func_02026784(void *game, int x, int y);
void func_02026794(void *game, void *obj);
int func_020267a0(void *game);
void func_020267ac(void *game, int a);
void func_020267b8(void *game, int a);
void func_020267c0(unsigned short *p, int a);
void func_020267c8(unsigned short *p, int a);
void func_020267d8(void *game, void *a, int b);
void func_020267f4(void *obj, int a);
void func_0202680c(void *a, int b);
void func_0202681c(void *a);
void func_02026824(void *a);
void func_0202682c(void *game);
void func_02026838(void *a, int b);
int func_02026844(void *a);
void func_02026854(void *game, int a);
void func_0202687c(void *game);
void func_020268b0(void *game);
void func_020268e0(void *a, signed char *name);
void func_020268ec(void *a);
void func_020268fc(void *a);
signed char *func_02026910(void *a);
int func_02026914(void *a);
int func_02026920(void *a);
void *func_02026924(void *mem);
void *func_02026968(void *mem);
void *func_0202697c(void *mem);
void *func_02026990(void *mem);
void *func_020269a4(void *mem);
void *func_020269b8(void *mem);
void *func_020269cc(void *mem);
void *func_020269e0(void *mem);
void *func_020269f4(void *mem);
void *func_02026a08(void *mem);
void *func_02026a3c(void *mem);
void *func_02026a50(void *mem);
void *func_02026a64(void *mem);
void *func_02026a78(void *mem);
void *func_02026a8c(void *mem);
void *func_02026aa0(void *mem);
void *func_02026ab4(void *mem);
void *func_02026ac8(void *mem);
void *func_02026adc(void *mem);
void *func_02026af0(void *mem);
void *func_02026b04(void *mem);
void *func_02026b18(void *mem);
void *func_02026b2c(void *mem);
int func_02027424(unsigned int flag);
void func_02027458(unsigned int flag);
void func_02027480(unsigned int flag);
void func_02027568(unsigned short *code, int a, int b);
void func_020275cc(unsigned short *code, int a, int b);
int func_02027630(int a, int b);
void func_020276fc(int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e, signed char *f, void *g, int h, int i, int j, int k, short l, int m, int n);
void func_020277d8(unsigned short *p);
void func_02027890(unsigned short *p);
void func_02027934(unsigned short *p);
void func_02027998(unsigned short *p, void *a);
void func_02027a68(unsigned short *p, void *a);
void func_02027b10(unsigned short *p);
void func_02027b74(unsigned short *p, Script *s);
signed char *func_02027c28(int id);
void *func_02027d4c(void *name, int a);
void func_02027f2c(Script *s, Vec *v);
void func_02027f74(Script *s, int a);
void func_02027fc4(Script *s, int a);
void func_02027ff0(Script *s, int a, int b, int c);
void func_0202804c(Script *s, int a, int b);
void func_02028074(Script *s, int a);
void func_020280c4(Script *s, int a, int b, int c);
void func_020280c8(Script *s, unsigned short *p, int a, int b);
void func_0202810c(Script *s, unsigned int a);
Actor *func_02028148(Script *s);
signed char *func_02028160(Script *s, int a);
Actor *func_02028170(Script *s, int a, int b);
int func_020281f0(Script *s, int a);
void func_02028410(Script *s, int a, int b, int c);
void *func_02028518(Script *s, unsigned short *p);
void func_0202852c(Script *s, int a);
void func_0202a080(void *obj, int a, int b);
void *func_0202a0c0(void *obj, int a);
void *func_0202a0ec(void *obj, int a, int b, int c);
void *func_0202a114(void *obj, int a, int b, int c);
void func_0202a16c(void *obj, int a);
void func_0202a450(void *obj, int a);
void func_0202a90c(void *obj, int a);
void func_0202aba4(void *obj, unsigned char a, int b);
void func_0202f7e8(void *list, void *obj, int a, int b);
int func_0202fa8c(void *list, char *name);
void func_0202fad4(void *list, void *obj);
Node *func_0202fb48(void *list, char *name, int a);
int func_0202fb8c(void *list, char *name, int a);
void func_0202fbcc(Iter *it, int a);
void *func_02031254(int size);
void func_0203ddc4(void *obj);
void func_0203f134(void *obj, int x, int y, unsigned int a);
void func_0203f1b8(void *obj, int x, int y);
void *func_0205176c(signed char *name);
void func_020521d4(char *name, int a, int b, int c);
void *func_02056964(char *name);
void func_02056d68(void *obj, int a, int b);
void func_02059020(void *obj);
void func_0205902c(void *obj, int a);
void func_020593f8(void *obj, void *a, int b, int c);
void func_02059430(void *obj, void *a, int b, int c);
void func_02059564(void *obj);
void func_0205958c(Vec *out, void *obj, int a);
void func_02059894(void *obj, void *a);
void func_02059bd8(void *obj, int a);
void func_020610ac(void *a, void *b, int c);
void func_0206115c(void *a, void *b, int c);
void func_020611b4(void *a, void *b, int c, int d);
void func_02061c10(void *a, void *b, int c);
Panel *func_02063b48(char *name);
void *func_02063b5c(char *name);
void func_02075fc4(void);
int func_020790c8(void *a, void *b);
int func_02023fdc(Script *s);
}

int func_02023fdc(Script *s)
{
    Vec v;
    Vec w;
    Vec u;
    Vec r;
    Vec r2;
    Vec r3;
    Vec pos;
    Iter it;
    Vec tmp;
    Doc doc;
    Doc doc2;
    Ids ids;
    unsigned int x0;
    unsigned int x1;
    unsigned int x2;
    unsigned int x3;
    short x4;
    int x5;
    int x6;
    int x7;
    int x8;
    int x9;
    int x10;
    int op;
    unsigned short *cmd;
    Cmd *o;
    void *obj;
    unsigned int flag;
    unsigned char bits;
    unsigned int i;
    int mask;
    unsigned int n;
    void *id;
    void *p;
    void *q;
    Actor *t;
    Actor *act;
    unsigned int a;
    unsigned int b;
    unsigned int c;
    Maker *m;
    void *x;
    void *y;
    signed char *name;
    void *kind;
    int k;
    void *e;
    Node *n2;
    int dx;
    int dy;
    int hx;
    int hy;
    Panel *g;
    void *h;
    unsigned int idx;
    const Vec *base;

    if (s->v6() != 0) {
        return 0;
    }
    if (s->wait > 0) {
        s->wait = s->wait - 1;
        return 0;
    }
    cmd = s->code + s->pc;
    func_0200f70c(&v);
    op = *cmd;
    switch (op) {
    case 0:
    case 1:
    case 0x112:
    case 0x113:
        return 0;
    case 8:
        func_02026534(func_02008444());
        s->pc++;
        break;
    case 9:
        func_02026544(func_02008444());
        s->pc++;
        break;
    case 0x1d:
        return s->v13();
    case 0x1b:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_02026924(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 4;
        break;
    case 0x1c:
        return s->v11();
    case 0x26:
        func_02027568(s->code, cmd[1], 1);
        s->pc += 2;
        break;
    case 0x27:
        func_02027568(s->code, cmd[1], 0);
        s->pc += 2;
        break;
    case 0x28:
        func_020275cc(s->code, cmd[1], 1);
        s->pc += 2;
        break;
    case 0x29:
        func_020275cc(s->code, cmd[1], 0);
        s->pc += 2;
        break;
    case 0x2a:
        func_020184f4(func_02008444(), cmd[1], cmd[2]);
        s->pc += 3;
        break;
    case 0x2b:
        func_020521d4(data_02084fc4, 0, 4, 0x100);
        s->pc++;
        break;
    case 0x67:
        func_02011094(func_02008444());
        s->pc++;
        return 0;
    case 0x68:
        func_020110ec(func_02008444());
        s->pc++;
        return 0;
    case 0x69:
        s->b10 = func_02011320(func_02008444()) == 2;
        s->pc++;
        break;
    case 0x6a:
        func_02011288(func_02008444());
        s->pc++;
        return 0;
    case 0x6b:
        s->b10 = func_0202657c(func_02008444());
        s->pc++;
        break;
    case 0x6c:
        if (func_0202657c(func_02008444()) != 0) {
            return 0;
        }
        s->pc++;
        break;
    case 0x6d:
        func_02013a24(func_02008444(), 0);
        s->pc++;
        break;
    case 0x6e:
        func_02013a24(func_02008444(), 1);
        s->pc++;
        break;
    case 0x6f:
        func_0201354c(func_02008444(), 0);
        func_02010ca4(func_02008444());
        s->pc++;
        break;
    case 0x70:
        func_0201354c(func_02008444(), 1);
        func_02010d18(func_02008444());
        s->pc++;
        break;
    case 0x71:
        func_0201354c(func_02008444(), 0);
        func_02010cdc(func_02008444());
        s->pc++;
        break;
    case 0x72:
        func_0201354c(func_02008444(), 1);
        func_02010d30(func_02008444());
        s->pc++;
        break;
    case 0x73:
        func_02010da4(func_02008444(), cmd[1] != 0);
        s->pc += 2;
        break;
    case 0x1e:
        s->v17(2);
        s->pc++;
        break;
    case 0x1f:
        s->v17(0);
        s->pc++;
        break;
    case 0x20:
        s->v17(3);
        s->pc++;
        break;
    case 0x21:
        s->v17(1);
        s->pc++;
        break;
    case 0x24:
        func_02010350(func_02008444(), func_02027c28(cmd[1]));
        s->pc += 2;
        break;
    case 0x15:
        s->pc += 4;
        break;
    case 0x30:
        func_0202fad4(func_02009900(), data_020cedc4);
        return 0;
    case 0x31:
        return s->v10();
    case 0x16:
        return s->v7();
    case 0x17:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_02026968(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 2;
        break;
    case 0x18:
        s->b10 = func_02028170(s, cmd[1], 1) ? 1 : 0;
        s->pc += 2;
        break;
    case 0x19:
        return s->v8();
    case 0x1a:
        return s->v9();
    case 0x32:
        s->b10 = func_02027424(func_020265c4(cmd + 1)) != 0;
        s->pc += 3;
        break;
    case 0x33:
        if (s->b10 != 0) {
            s->pc += 4;
        } else {
            s->pc += 1;
        }
        break;
    case 0x114:
        s->b10 = 1;
        s->pc++;
        break;
    case 0x34:
        if (s->b10 == 0) {
            s->pc += 4;
        } else {
            s->pc += 1;
        }
        break;
    case 0x115:
        s->b10 = 0;
        s->pc++;
        break;
    case 0x61:
        func_02027458(func_020265c4(cmd + 1));
        s->pc += 3;
        break;
    case 0x62:
        func_02027480(func_020265c4(cmd + 1));
        s->pc += 3;
        break;
    case 0x63:
        flag = func_020265c4(cmd + 1);
        bits = (unsigned char)cmd[3];
        for (i = 0; i < 8; i++, flag++, bits >>= 1) {
            if (bits & 1) {
                func_02027458(flag);
            } else {
                func_02027480(flag);
            }
        }
        s->pc += 4;
        break;
    case 0x64:
        flag = func_020265c4(cmd + 1);
        mask = 0;
        for (i = 0; i < 8; i++) {
            if (func_02027424(flag) != 0) {
                mask |= (unsigned char)(1 << i);
            }
            flag++;
        }
        s->f18 = mask;
        s->pc += 3;
        break;
    case 0x65:
        func_02027b74(cmd, s);
        s->pc += 5;
        break;
    case 0x66:
        n = func_020265dc(cmd + 1);
        if (func_01ff80b8(func_02008eb0()) % n == 0) {
            s->b10 = 1;
        } else {
            s->b10 = 0;
        }
        s->pc += 2;
        break;
    case 0x74:
        func_020125d4(func_02008444(), cmd[1]);
        s->pc += 2;
        break;
    case 0x75:
        func_020265ec(func_02008444(), cmd[1]);
        obj = func_0201ff10(func_02008444(), 2);
        if (obj != 0) {
            func_0201fcd0(obj, cmd[1]);
        }
        obj = func_0201ff10(func_02008444(), 0);
        if (obj != 0) {
            func_0201fcd0(obj, cmd[1]);
        }
        s->pc += 2;
        break;
    case 0x76:
        obj = func_0201ff10(func_02008444(), cmd[1]);
        if (obj != 0 && cmd[2] != 0) {
            func_0201f974(obj, cmd[2], cmd[3], -1);
            if (cmd[1] == 2 && cmd[2] == func_020265fc(func_02008444())) {
                obj = func_0201ff10(func_02008444(), 2);
                if (obj != 0) {
                    func_0201fcd0(obj, cmd[2]);
                }
            }
            if (cmd[1] == 0 && cmd[2] == func_020265fc(func_02008444())) {
                obj = func_0201ff10(func_02008444(), 0);
                if (obj != 0) {
                    func_0201fcd0(obj, cmd[2]);
                }
            }
        }
        s->pc += 4;
        break;
    case 0x78:
        obj = func_0201ff10(func_02008444(), cmd[1]);
        if (obj != 0) {
            s->b10 = func_0201fa5c(obj, cmd[2], cmd[3]);
        }
        s->pc += 4;
        break;
    case 0x77:
        func_02026608(func_02008444(), cmd[2]);
        func_020123a0(func_02008444(), cmd[1]);
        s->pc += 3;
        break;
    case 0x79:
        func_0202661c(func_02008444());
        s->pc++;
        break;
    case 0x7a:
        s->b10 = func_0202662c(func_02008444());
        s->pc++;
        if (func_0202662c(func_02008444()) == 0) {
            break;
        }
        id = func_02027d4c(data_02084fd0, 2);
        func_02015a54(func_02008444(), id, 1);
        return 0;
    case 0x7b:
        s->b10 = func_0202662c(func_02008444());
        s->pc++;
        break;
    case 0x7c:
        func_0202663c(func_02008444(), func_02027c28(cmd[1]));
        s->pc += 2;
        break;
    case 0x7d:
        s->pc += 2;
        break;
    case 0x7e:
        func_0202665c(func_02008444());
        s->pc++;
        break;
    case 0x7f:
        func_02015748(func_0201003c());
        s->pc++;
        break;
    case 0x80:
        func_02015418(func_0201003c());
        s->pc++;
        break;
    case 0x22:
    case 0x23:
        s->pc = (func_020265c4(cmd + 1) - 0x34) >> 1;
        break;
    case 0x2d:
        if (func_02026668(s, s->pc + 3) != 0) {
            s->pc = (func_020265c4(cmd + 1) - 0x34) >> 1;
        } else {
            func_02075fc4();
        }
        break;
    case 0x2e:
        s->pc = func_02026680(s);
        break;
    case 0x37:
        id = func_02028518(s, cmd + 1);
        func_02015a54(func_02008444(), id, 0);
        s->pc += 3;
        return 0;
    case 0x38:
        id = func_02027d4c(func_02027c28(cmd[1]), 2);
        func_02015a54(func_02008444(), id, 1);
        s->pc += 2;
        return 0;
    case 0x39:
        p = func_02022f04(func_0201123c(func_02011264(), 0), -1);
        if (func_02008828(p, data_020cd798) != 0) {
            return 0;
        }
        if (func_02011244(func_02008444()) == 0 && func_0202fa8c(func_02009900(), data_020cf980) == 0) {
            s->pc++;
            break;
        }
        return 0;
    case 0x3a:
        func_02015b70(func_0201003c());
        s->pc++;
        break;
    case 0x3b:
        func_02010384(func_02008444(), 1);
        p = func_0201ee30(data_02084fe0, data_020bbbb4);
        if (p != 0 && func_02022fc0(func_0201123c(func_02011264(), 0), p) == 0
            && func_02022fa4(func_0201123c(func_02011264(), 0), data_02084fec) == 0) {
            func_02022f34(func_0201123c(func_02011264(), 0), data_02084fec, 0);
        }
        func_02015b98(func_0201003c());
        s->pc++;
        return 0;
    case 0x3c:
        a = func_020265c4(cmd + 5);
        func_02011da4(func_02008444(), func_02028518(s, cmd + 1), func_02028518(s, cmd + 3), a);
        s->pc += 7;
        break;
    case 0x3d:
        a = func_020265c4(cmd + 6);
        func_02011e48(func_02008444(), func_02028518(s, cmd + 2), func_02028518(s, cmd + 4), a, cmd[1]);
        s->pc += 8;
        break;
    case 0x3e:
        func_02011d1c(func_02008444());
        s->pc++;
        break;
    case 0x3f:
        func_02011d74(func_02008444());
        s->pc++;
        break;
    case 0x40:
        a = func_020265c4(cmd + 3);
        func_02011f34(func_02008444(), func_02028518(s, cmd + 1), a, func_02027c28(cmd[5]), func_02027c28(cmd[6]), func_02027c28(cmd[7]));
        s->pc += 8;
        break;
    case 0x41:
        a = func_020265c4(cmd + 4);
        func_02011fac(func_02008444(), func_02028518(s, cmd + 2), a, func_02027c28(cmd[6]), func_02027c28(cmd[7]), func_02027c28(cmd[8]), cmd[1]);
        s->pc += 9;
        break;
    case 0x42:
        func_02015bb4(func_0201003c());
        s->pc++;
        break;
    case 0x43:
        func_02015bcc(func_0201003c());
        s->pc++;
        break;
    case 0x44:
        s->pc++;
        break;
    case 0x45:
        if (func_020265c4(cmd + 1) != 0) {
            t = func_02028148(s);
            if (t != 0 && func_02008828(t, data_020cf898) != 0) {
                a = func_020265c4(cmd + 3);
                b = func_020265c4(cmd + 5);
                func_02010394(func_02008444(), t, func_02028518(s, cmd + 1), a, b);
            }
        } else {
            func_02010754(func_02008444());
        }
        s->pc += 7;
        break;
    case 0x46:
        a = func_020265c4(cmd + 1);
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf898) != 0) {
            b = func_020265c4(cmd + 6);
            func_02010440(func_02008444(), t, a, func_02027c28(cmd[3]), func_02027c28(cmd[4]), func_02027c28(cmd[5]), b);
        }
        s->pc += 8;
        break;
    case 0x47:
        a = func_020265c4(cmd + 1);
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf898) != 0) {
            b = func_020265c4(cmd + 3);
            c = func_020265c4(cmd + 5);
            func_020104b4(func_02008444(), t, a, b, c);
        }
        s->pc += 7;
        break;
    case 0x48:
        func_020106d8(func_02008444(), cmd[1]);
        s->pc += 2;
        break;
    case 0x49:
        func_020106f8(func_02008444());
        s->pc++;
        break;
    case 0x4a:
        func_020106b8(func_02008444(), cmd[1]);
        s->pc += 2;
        break;
    case 0x4b:
        s->b10 = func_02026698(func_02008444());
        s->pc++;
        break;
    case 0x4c:
        a = func_020265c4(cmd + 5);
        func_0201052c(func_02008444(), func_02028518(s, cmd + 1), (short)cmd[3], cmd[4], a);
        s->pc += 7;
        break;
    case 0x4e:
        a = func_020265c4(cmd + 6);
        func_020105d4(func_02008444(), (short)cmd[1], cmd[2], func_02027c28(cmd[3]), func_02027c28(cmd[4]), func_02027c28(cmd[5]), a);
        s->pc += 8;
        break;
    case 0x4f:
        a = func_020265c4(cmd + 1);
        b = func_020265c4(cmd + 5);
        func_0201062c(func_02008444(), a, (short)cmd[3], cmd[4], b);
        s->pc += 7;
        break;
    case 0x4d:
        func_020106a0(func_02008444());
        s->pc++;
        break;
    case 0x50:
        s->b10 = func_020266ac(func_02008444());
        s->pc++;
        break;
    case 0x51:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf898) != 0) {
            func_02010778(func_02008444(), t, func_02028518(s, cmd + 1), cmd[3], (short)cmd[4]);
        }
        s->pc += 5;
        break;
    case 0x52:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf898) != 0) {
            func_020265c4(cmd + 1);
            func_02010778(func_02008444(), t, 0, cmd[1], (short)cmd[2]);
        }
        s->pc += 3;
        break;
    case 0x53:
    case 0x54:
    case 0x55:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf898) != 0) {
            v.x = func_020265c4(cmd + 1);
            v.y = func_020265c4(cmd + 3);
            func_02010ba0(func_02008444(), t, v.x, v.y, *cmd - 0x53);
        }
        s->pc += 5;
        break;
    case 0x56:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf898) != 0) {
            func_02010c28(func_02008444(), t, cmd[1], cmd[2]);
        }
        s->pc += 3;
        break;
    case 0x57:
        func_02010824(func_02008444(), 0);
        s->pc++;
        break;
    case 0x58:
        func_02010974(func_02008444(), 1);
        s->pc++;
        break;
    case 0x59:
        func_02010974(func_02008444(), 0);
        s->pc++;
        break;
    case 0x5a:
        t = func_02028148(s);
        if (func_02010c70(func_02008444(), t) != 0) {
            return 0;
        }
        s->pc++;
        break;
    case 0x5b:
        func_02010874(func_02008444());
        s->pc++;
        break;
    case 0x2f:
        s->wait = cmd[1] >> data_020cff60.b1;
        s->pc += 2;
        return 0;
    case 0xa:
        if (func_020266c0() != 0) {
            return 0;
        }
        s->pc++;
        break;
    case 0xb:
    case 0xc:
    case 0xd:
    case 0xe:
        op -= 0xb;
        if (op == 0) {
            if (data_020c9e88.f12 > 0) {
                op = 2;
            }
        } else if (op == 2) {
            if (data_020c9e88.f12 < 0) {
                op = 0;
            }
        }
        switch (op) {
        case 0:
            func_020521d4(data_02084ffc, 0, cmd[1], 0x100);
            break;
        case 1:
            func_020521d4(data_02085008, 0, cmd[1], 0x100);
            break;
        case 2:
            func_020521d4(data_02084fc4, 0, cmd[1], 0x100);
            break;
        case 3:
            func_020521d4(data_02085014, 0, cmd[1], 0x100);
            break;
        }
        if (func_020112a8(func_02008444()) != 0) {
            switch (op) {
            case 0:
                func_020521d4(data_02084ffc, 1, cmd[1], 0x100);
                break;
            case 1:
                func_020521d4(data_02085008, 1, cmd[1], 0x100);
                break;
            case 2:
                func_020521d4(data_02084fc4, 1, cmd[1], 0x100);
                break;
            case 3:
                func_020521d4(data_02085014, 1, cmd[1], 0x100);
                break;
            }
        }
        s->pc += 2;
        break;
    case 0xf:
        e = func_0202fb48(func_02009900(), data_020cf75c, -1);
        if (e == 0) {
            e = func_02056964(data_020cf75c);
            func_0202f7e8(func_02009900(), e, 5, 0);
        }
        if (e != 0) {
            func_02056d68(e, (short)cmd[1], 0);
        }
        s->pc += 2;
        break;
    case 0x10:
        e = func_0202fb48(func_02009900(), data_020cf75c, -1);
        if (e == 0) {
            e = func_02056964(data_020cf75c);
            func_0202f7e8(func_02009900(), e, 5, 0);
        }
        if (e != 0) {
            func_02056d68(e, (short)cmd[1], 0x1f);
        }
        s->pc += 2;
        break;
    case 0x11:
        e = func_0202fb48(func_02009900(), data_020cf75c, -1);
        if (e == 0) {
            e = func_02056964(data_020cf75c);
            func_0202f7e8(func_02009900(), e, 5, 0);
        }
        if (e != 0) {
            func_0200ee34(e, 0x1f, 0x1f, 0x1f);
            func_02056d68(e, (short)cmd[1], 0);
        }
        s->pc += 2;
        break;
    case 0x12:
        e = func_0202fb48(func_02009900(), data_020cf75c, -1);
        if (e == 0) {
            e = func_02056964(data_020cf75c);
            func_0202f7e8(func_02009900(), e, 5, 0);
        }
        if (e != 0) {
            func_0200ee34(e, 0x1f, 0x1f, 0x1f);
            func_02056d68(e, (short)cmd[1], 0x1f);
        }
        s->pc += 2;
        break;
    case 0x13:
        func_0200e930(func_02008444(), cmd[1]);
        s->pc += 2;
        break;
    case 0x14:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_0202697c(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 3;
        break;
    case 0x81:
        return s->v12();
    case 0x82:
        m = (Maker *)func_0201ee6c(func_020265c4(cmd + 1), data_020bbbb4);
        if (m != 0) {
            s->f14 = m->make();
            func_0202f7e8(func_02009900(), s->f14, 6, 0);
            if (s->f14 != 0) {
                p = func_0202fb48(func_02009900(), data_020cedc4, -1);
                if (p != 0) {
                    func_020153d8(s->f14, p);
                }
            }
        }
        s->pc += 3;
        break;
    case 0x83:
        x = func_0205176c(func_02028160(s, cmd[1]));
        if (x != 0) {
            s->f14 = func_020266d4(x);
        }
        s->pc += 2;
        break;
    case 0x84:
        p = func_0201ee6c(func_020265c4(cmd + 1), data_020bbbb4);
        if (p != 0) {
            func_0202fad4(func_02009900(), p);
        }
        s->pc += 3;
        break;
    case 0x85:
        name = func_02028160(s, cmd[1]);
        if (name[0] != '*') {
            x = func_0205176c(name);
            if (x != 0) {
                y = func_020082f8(func_020266d4(x), data_020cf898);
                if (y != 0) {
                    t = func_02028148(s);
                    if (t != 0) {
                        func_020593f8(t, y, cmd[2], cmd[3]);
                    }
                }
            }
        } else {
            t = func_02028148(s);
            if (t != 0 && func_02008828(t, data_020cf898) != 0) {
                func_020593f8(t, 0, 0, 0);
            }
        }
        s->pc += 4;
        break;
    case 0x86:
        name = func_02028160(s, cmd[1]);
        if (name[0] != '*') {
            x = func_0205176c(name);
            if (x != 0) {
                y = func_020082f8(func_020266d4(x), data_020cf898);
                if (y != 0) {
                    t = func_02028148(s);
                    if (t != 0) {
                        func_02059430(t, y, cmd[2], cmd[3]);
                    }
                }
            }
        }
        s->pc += 4;
        break;
    case 0x116:
        s->f14 = 0;
        s->pc++;
        break;
    case 0x87:
        v.x = func_020265c4(cmd + 1);
        v.y = func_020265c4(cmd + 3);
        v.z = func_020265c4(cmd + 5);
        func_02027f2c(s, &v);
        s->pc += 7;
        break;
    case 0x8c:
        func_02027f74(s, cmd[1]);
        s->pc += 2;
        break;
    case 0x8d:
        func_0202810c(s, func_020265c4(cmd + 1));
        s->pc += 3;
        break;
    case 0x91:
        func_0202804c(s, (short)cmd[1], (short)cmd[2]);
        s->pc += 3;
        break;
    case 0x92:
    case 0x93:
        func_0202852c(s, op - 0x92);
        s->pc += 5;
        break;
    case 0x8e:
        func_02027ff0(s, (short)cmd[1], (short)cmd[2], 0);
        s->pc += 3;
        break;
    case 0x8f:
        func_02027fc4(s, cmd[1]);
        s->pc += 2;
        break;
    case 0x90:
        func_02027ff0(s, (short)cmd[1], (short)cmd[2], cmd[3]);
        s->pc += 4;
        break;
    case 0x94:
        t = func_02028148(s);
        if (t != 0) {
            t->a10();
            if (func_02008828(t, data_020cf898) != 0) {
                t->a15();
                func_02059564(t);
            }
        }
        s->pc++;
        break;
    case 0x95:
        t = func_02028148(s);
        if (t != 0) {
            t->a11();
            if (func_02008828(t, data_020cf898) != 0) {
                func_02059020(t);
            }
        }
        s->pc++;
        break;
    case 0x96:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_02026990(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 2;
        break;
    case 0x97:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_020269a4(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 2;
        break;
    case 0x98:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_020269b8(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 2;
        break;
    case 0x99:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_020269cc(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 2;
        break;
    case 0x9a:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf898) != 0) {
            func_0205902c(t, (short)cmd[1]);
        }
        s->pc += 2;
        break;
    case 0x9b:
        func_02028074(s, cmd[1]);
        s->pc += 2;
        break;
    case 0x9c:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf898) != 0) {
            func_020101c0(func_02008444(), t, cmd[1], 0);
        }
        s->pc += 2;
        break;
    case 0xa4:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf898) != 0) {
            func_020101c0(func_02008444(), t, cmd[1], 1);
            p = func_0202fb48(func_02009900(), data_020cfb90, -1);
            if (p != 0) {
                func_020611b4(p, t, cmd[1], 0);
            }
        }
        s->pc += 2;
        break;
    case 0xa7:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf898) != 0) {
            func_020101c0(func_02008444(), t, cmd[1], 2);
            p = func_0202fb48(func_02009900(), data_020cfb90, -1);
            if (p != 0) {
                func_0206115c(p, t, cmd[1]);
            }
        }
        s->pc += 2;
        break;
    case 0xa5:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf87c) != 0) {
            x = func_020266dc(t, cmd[1]);
            if (x != 0) {
                func_020266e8(func_02008444(), x);
            }
            p = func_0202fb48(func_02009900(), data_020cfb90, -1);
            if (p != 0) {
                func_020611b4(p, t, cmd[1], 1);
            }
        }
        s->pc += 2;
        break;
    case 0xa6:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf87c) != 0) {
            p = func_0202fb48(func_02009900(), data_020cfb90, -1);
            if (p != 0) {
                func_020610ac(p, t, cmd[1]);
            }
        }
        s->pc += 2;
        break;
    case 0x9d:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_020269e0(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 3;
        break;
    case 0x9e:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_020269f4(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 3;
        break;
    case 0x9f:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf87c) != 0) {
            idx = cmd[1];
            if (idx < func_020266f4(t)) {
                func_02026700(func_020266dc(t, idx), cmd[2]);
            }
        }
        s->pc += 3;
        break;
    case 0xa0:
        s->b10 = func_02027630(cmd[1], -1);
        s->pc += 2;
        break;
    case 0xa1:
        s->b10 = func_02027630(cmd[1], 0);
        s->pc += 2;
        break;
    case 0xa2:
        s->b10 = func_02027630(cmd[1], 1);
        s->pc += 2;
        break;
    case 0xa3:
        s->b10 = func_02027630(cmd[1], 2);
        s->pc += 2;
        break;
    case 0xa8:
        s->b10 = 0;
        s->pc += 2;
        break;
    case 0xa9:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf898) != 0 && t->a12() != 0) {
            s->pc++;
            return 1;
        }
        return 0;
    case 0xaa:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf8b4) != 0) {
            func_02026734(t, cmd[1]);
            s->pc += 2;
            return 1;
        }
        s->pc += 3;
        return 0;
    case 0xab:
        func_020280c4(s, cmd[1], cmd[2], cmd[3]);
        s->pc += 4;
        break;
    case 0xac:
        t = func_02028148(s);
        if (t != 0) {
            func_02008418(t);
        }
        s->pc++;
        break;
    case 0xad:
        t = func_02028148(s);
        if (t != 0) {
            func_02010dc4(t);
        }
        s->pc++;
        break;
    case 0xae:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf8b4) != 0) {
            func_02059bd8(t, cmd[1]);
        }
        s->pc += 2;
        break;
    case 0xaf:
        act = func_02028170(s, cmd[1], 0);
        if (func_02008828(act, data_020cf8b4) != 0) {
            func_02059020(act);
            act->a15();
        }
        s->pc += 2;
        break;
    case 0x117:
        a = func_020265c4(cmd + 1);
        b = cmd[3] != 0;
        c = cmd[4] != 0;
        func_020280c8(s, &s->code[(a - 0x34) >> 1], b, c);
        s->pc += 5;
        break;
    case 0x118:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf8b4) != 0) {
            func_02026744(t, cmd[1]);
        }
        s->pc += 2;
        break;
    case 0x119:
        act = func_02028170(s, cmd[1], 0);
        t = func_02028148(s);
        if (t != 0) {
            func_02059894(t, act);
        }
        s->pc += 2;
        break;
    case 0xb0:
        x0 = func_020265c4(cmd + 2);
        x1 = func_020265c4(cmd + 4);
        x2 = func_020265c4(cmd + 6);
        x3 = func_020265c4(cmd + 8);
        x4 = (short)cmd[0xb];
        x5 = (short)cmd[0xc];
        x6 = cmd[0xd];
        x7 = cmd[0xe];
        x8 = cmd[0xf];
        x9 = cmd[0x10];
        x10 = (short)cmd[0x11];
        t = func_02028148(s);
        kind = 0;
        if (t != 0) {
            kind = func_020082f8(t, data_020cf898);
        }
        func_020276fc(cmd[1], x0, x1, x2, x3, func_02027c28(cmd[0xa]), kind, x4, x5, x6, x7, x8, x9, x10);
        s->pc += 0x12;
        break;
    case 0xb1:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_02026a08(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 2;
        break;
    case 0xb2:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_02026a3c(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 2;
        break;
    case 0xb3:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_02026a50(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 3;
        break;
    case 0xb4:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_02026a64(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 6;
        break;
    case 0xb5:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_02026a78(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 3;
        break;
    case 0xb6:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_02026a8c(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 6;
        break;
    case 0xb7:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_02026aa0(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 7;
        break;
    case 0xb8:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_02026ab4(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 3;
        break;
    case 0xb9:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_02026ac8(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 3;
        break;
    case 0xba:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_02026adc(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 3;
        break;
    case 0xbb:
        func_020277d8(cmd);
        s->pc += 0x12;
        break;
    case 0xbc:
        func_02027890(cmd);
        s->pc += 0xa;
        break;
    case 0xbd:
        func_02027934(cmd);
        s->pc += 2;
        break;
    case 0xbe:
        kind = 0;
        if ((short)cmd[0xb] >= 0) {
            t = func_02028148(s);
            if (t != 0) {
                kind = func_020082f8(t, data_020cf898);
            }
        }
        func_02027998(cmd, kind);
        s->pc += 0xe;
        break;
    case 0xbf:
        kind = 0;
        t = func_02028148(s);
        if (t != 0) {
            kind = func_020082f8(t, data_020cf898);
        }
        func_02027a68(cmd, kind);
        s->pc += 0xa;
        break;
    case 0xc0:
        func_02027b10(cmd);
        s->pc += 2;
        break;
    case 0xc1:
        func_020082d8(&w, func_0201230c(func_02008444()));
        if (cmd[2] == 0xffff) {
            w.x += func_0200f178(func_02008444());
        } else {
            w.x += (short)cmd[2] << 12;
        }
        if (cmd[3] == 0xffff) {
            w.y += func_0200f190(func_02008444());
        } else {
            w.y += (short)cmd[3] << 12;
        }
        func_02011a14(func_02008444(), w.x);
        func_02011a28(func_02008444(), w.y);
        func_0200ecb0(func_02008444(), cmd[1], 0x80000);
        s->pc += 4;
        if (cmd[5] == 0xc3) {
            func_0200eee0(func_02008444());
            s->pc++;
        }
        return 0;
    case 0xc2:
        if (func_0200eec4(func_02008444()) == 0) {
            return 0;
        }
        s->pc++;
        break;
    case 0xc3:
        func_0200eee0(func_02008444());
        s->pc++;
        break;
    case 0xc4:
        func_0200eef4(func_02008444());
        s->pc++;
        break;
    case 0xc5:
        p = func_02022f04(func_0201123c(func_02011264(), 1), -1);
        if (p != 0) {
            q = func_020082f8(p, data_020ce1e8);
            if (q != 0) {
                func_02026770(q, cmd[1], (short)cmd[2] << 12, (short)cmd[3] << 12);
            }
        }
        s->pc += 4;
        return 1;
    case 0xc6:
        p = func_02022f04(func_0201123c(func_02011264(), 1), -1);
        if (p != 0) {
            q = func_020082f8(p, data_020ce1e8);
            if (q != 0) {
                func_0203f1b8(q, (short)cmd[1] << 12, (short)cmd[2] << 12);
            }
        }
        s->pc += 3;
        break;
    case 0xc7:
        a = func_020265c4(cmd + 3);
        func_020082d8(&u, func_0201230c(func_02008444()));
        if (cmd[1] == 0xffff) {
            u.x = func_0200f178(func_02008444());
        } else {
            u.x += (short)cmd[1] << 12;
        }
        if (cmd[2] == 0xffff) {
            u.y = func_0200f190(func_02008444());
        } else {
            u.y += (short)cmd[2] << 12;
        }
        func_0200f604(func_02008444(), u.x, u.y, a);
        s->pc += 5;
        break;
    case 0xc8:
        func_020082d8(&r, func_0201230c(func_02008444()));
        func_02026784(func_02008444(), (cmd[1] << 12) + r.x, (cmd[2] << 12) + r.y);
        func_0200f918(func_02008444(), 1);
        s->pc += 3;
        break;
    case 0xc9:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf860) != 0) {
            func_02026794(func_02008444(), t);
        }
        s->pc++;
        break;
    case 0xca:
        a = func_020265c4(cmd + 1);
        func_0200f634(func_02008444(), a);
        s->pc += 3;
        break;
    case 0xcb:
        func_0200f780(func_02008444(), 1);
        s->pc++;
        break;
    case 0xcc:
        func_0200f780(func_02008444(), 0);
        s->pc++;
        break;
    case 0xcd:
        a = func_020265c4(cmd + 1);
        func_0200f958(func_02008444(), a);
        s->pc += 3;
        break;
    case 0xce:
        act = func_02028170(s, cmd[1], 0);
        if (act != 0 && func_02008828(act, data_020cf898) != 0) {
            a = func_020265c4(cmd + 5);
            func_0200faa4(func_02008444(), act, (short)cmd[2], (short)cmd[3], (short)cmd[4], a);
        }
        s->pc += 7;
        break;
    case 0xcf:
        if (func_020267a0(func_02008444()) != 0) {
            return 0;
        }
        s->pc++;
        break;
    case 0xd0:
        func_020141f4(func_02008444());
        func_0200f918(func_02008444(), 1);
        s->pc++;
        break;
    case 0xd1:
        n2 = func_0202fb48(func_02009900(), data_020cedc4, -1);
        if (n2 != 0) {
            func_02020c98(&doc);
            n2->n3(&doc);
            e = func_02020db0(&doc, data_02085020);
            if (e != 0) {
                func_02020258(e, func_0200f178(func_02008444()));
            }
            e = func_02020db0(&doc, data_02085028);
            if (e != 0) {
                func_02020258(e, func_0200f190(func_02008444()));
            }
            func_02020ca8(&doc);
        }
        s->pc++;
        break;
    case 0xd2:
        n2 = func_0202fb48(func_02009900(), data_020cedc4, -1);
        if (n2 != 0) {
            func_02020c98(&doc2);
            n2->n3(&doc2);
            dx = 0;
            dy = 0;
            e = func_02020db0(&doc2, data_02085020);
            if (e != 0) {
                dx = func_02020ac8(e);
            }
            e = func_02020db0(&doc2, data_02085028);
            if (e != 0) {
                dy = func_02020ac8(e);
            }
            func_020082d8(&r2, func_0201230c(func_02008444()));
            func_02026784(func_02008444(), dx + r2.x, dy + r2.y);
            func_0200f918(func_02008444(), 1);
            func_02020ca8(&doc2);
        }
        s->pc++;
        break;
    case 0xd3:
        func_0200fa64(func_02008444(), cmd[1], cmd[2]);
        s->pc += 3;
        break;
    case 0xd4:
        a = func_020265c4(cmd + 3);
        p = func_02022f04(func_0201123c(func_02011264(), 1), -1);
        if (p != 0) {
            q = func_020082f8(p, data_020ce1e8);
            if (q != 0) {
                func_0203f134(q, (short)cmd[1] << 12, (short)cmd[2] << 12, a);
            }
        }
        s->pc += 5;
        break;
    case 0xd5:
        func_02009bcc(func_02008444(), cmd[1], cmd[2]);
        s->pc += 3;
        break;
    case 0xd6:
        func_02009bbc(func_02008444());
        s->pc++;
        break;
    case 0xd7:
        func_020082d8(&r3, func_0201230c(func_02008444()));
        hx = cmd[1];
        hy = cmd[2];
        func_02009bdc(func_02008444(), hx, hy);
        func_020267ac(func_02008444(), ((short)hx << 12) + func_0200f178(func_02008444()) - r3.x);
        func_020267b8(func_02008444(), ((short)hy << 12) + func_0200f190(func_02008444()) - r3.y);
        s->pc += 3;
        break;
    case 0x127:
        s->f18 = func_020281f0(s, cmd[1]);
        s->pc += 2;
        break;
    case 0x128:
        func_02028410(s, cmd[1], func_020265c4(cmd + 2), 0);
        s->pc += 4;
        break;
    case 0x12c:
        func_020267c0(cmd + cmd[1], (unsigned short)s->f18);
        s->pc += 2;
        break;
    case 0x12d:
        func_020267c8(cmd + cmd[1], s->f18);
        s->pc += 2;
        break;
    case 0x129:
        func_02028410(s, cmd[1], 1, 1);
        s->pc += 2;
        break;
    case 0x12a:
        func_02028410(s, cmd[1], -1, 1);
        s->pc += 2;
        break;
    case 0x12b:
        func_02028410(s, cmd[1], 0, 0);
        s->pc += 2;
        break;
    case 0xd8:
        func_020101fc(func_02008444(), cmd[1]);
        s->pc += 2;
        break;
    case 0xd9:
        func_02010264(func_02008444(), (short)cmd[1]);
        s->pc += 2;
        break;
    case 0xda:
        func_02011074(func_02008444());
        s->pc++;
        return 0;
    case 0x5d:
        func_020118a8(func_02008444(), func_02028518(s, cmd + 1));
        s->pc += 3;
        return 0;
    case 0x5e:
        if (func_0202fb8c(func_02009900(), data_02085030, -1) == 0) {
            s->pc++;
        }
        return 0;
    case 0x5c:
        k = (short)cmd[3];
        if (k != -1) {
            k = 0;
        }
        p = func_02028518(s, cmd + 1);
        if (p != 0) {
            func_02011958(func_02008444(), p, k);
        } else {
            func_02015a34(func_0201003c());
        }
        s->pc += 4;
        return 0;
    case 0xdb:
        func_020267d8(func_02008444(), func_02028518(s, cmd + 1), cmd[3] != 0);
        func_02011cdc(func_02008444(), 0);
        s->pc += 4;
        return 0;
    case 0xdc:
        s->pc += 3;
        return 0;
    case 0xdd:
        if (func_02011320(func_02008444()) == 2) {
            func_0202f7e8(func_02009900(), func_02063b5c(data_020cfcf0), 1, 0);
        }
        s->pc++;
        break;
    case 0xde:
        if (func_0202fa8c(func_02009900(), data_020cfcf0) != 0) {
            func_0202fad4(func_02009900(), data_020cfcf0);
            func_0200c82c(func_02008860(), 1);
        }
        s->pc++;
        break;
    case 0xdf:
    case 0xe0:
        func_0200f084(func_02008444(), op - 0xdf, cmd[1] != 0, 1, 1);
        s->pc += 2;
        break;
    case 0xe1:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf898) != 0) {
            func_020267f4(t, 1);
        }
        s->pc++;
        break;
    case 0xe2:
        if (cmd[1] == 0) {
            func_02022f34(func_0201123c(func_02011264(), 0), data_02085048, 0);
        } else {
            func_02022f34(func_0201123c(func_02011264(), 0), data_02085064, 0);
        }
        func_02010384(func_02008444(), 1);
        s->v17(2);
        s->pc += 2;
        return 0;
    case 0xe3:
        func_0201192c(func_02008444(), 2);
        func_02022f34(func_0201123c(func_02011264(), 0), data_02085078, 0);
        s->v17(0);
        s->pc++;
        break;
    case 0xe4:
        return 0;
    case 0xe5:
    case 0xe6:
        func_02010074(func_02008444(), op - 0xe5 != 0);
        s->pc++;
        break;
    case 0xe7:
    case 0xe8:
        func_020100a0(func_02008444(), op - 0xe7 != 0);
        s->pc++;
        break;
    case 0xe9:
    case 0xea:
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf898) != 0) {
            idx = cmd[1];
            if (idx < func_020266f4(t)) {
                func_0202680c(func_020266dc(t, idx), *cmd == 0xea);
            }
        }
        s->pc += 2;
        break;
    case 0xeb:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_02026af0(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 2;
        break;
    case 0xec:
        p = func_0201ee6c(func_020265c4(cmd + 1), data_020bbbb4);
        if (p != 0) {
            func_0200ec24(func_02008444(), 1);
            func_02022f2c(func_0201123c(func_02011264(), 0), p, 0);
        }
        s->pc += 3;
        return 0;
    case 0xed:
        if (func_02022fa4(func_0201123c(func_02011264(), 0), data_02085088) != 0) {
            return 0;
        }
        s->pc++;
        break;
    case 0xee:
        o = (Cmd *)func_02031254(4);
        if (o != 0) {
            o = (Cmd *)func_02026b04(o);
        }
        if (o != 0) {
            o->c2(s, func_02027c28(cmd[1]));
            if (o != 0) {
                o->c1();
            }
        }
        s->pc += 2;
        break;
    case 0xef:
        func_02022f34(func_0201123c(func_02011264(), 0), data_02085098, 0);
        s->pc++;
        return 0;
    case 0xf0:
        if (func_02022fa4(func_0201123c(func_02011264(), 0), data_02085098) != 0) {
            return 0;
        }
        s->pc++;
        break;
    case 0xf1:
        data_020cff60.b0 = cmd[1];
        func_02022f34(func_0201123c(func_02011264(), 0), data_020850b0, 0);
        s->pc += 2;
        return 0;
    case 0xf2:
        if (func_02022fa4(func_0201123c(func_02011264(), 0), data_020850b0) != 0) {
            return 0;
        }
        s->pc++;
        break;
    case 0xf3:
    case 0xf4:
        func_0202fad4(func_02009900(), data_020cfd28);
        g = (Panel *)func_02063b48(data_020cfd28);
        if (g != 0) {
            ids = data_02084fbc;
            if (cmd[1] <= 2) {
                g->p33(ids.id[cmd[1]]);
            }
            if (*cmd == 0xf4) {
                func_0202681c(g);
            }
            func_02026824(g);
            func_0202f7e8(func_02009900(), g, 0xd, 0);
        }
        s->pc += 2;
        func_0202682c(func_02008444());
        return 0;
    case 0xf5:
        func_0202fad4(func_02009900(), data_020cfd28);
        s->pc++;
        break;
    case 0xf6:
        p = func_0202fb48(func_02009900(), data_020cfd28, -1);
        if (p != 0) {
            func_02026838(p, (short)cmd[1]);
        }
        s->pc += 2;
        break;
    case 0xf7:
        name = func_02027c28(cmd[1]);
        if (func_020790c8(name, data_020850c8) == 0) {
            func_0200f1a8(func_02008444(), cmd[2] << 4);
        } else {
            o = (Cmd *)func_02031254(4);
            if (o != 0) {
                o = (Cmd *)func_02026b18(o);
            }
            if (o != 0) {
                o->c2(s, name);
                if (o != 0) {
                    o->c1();
                }
            }
            o = (Cmd *)func_02031254(4);
            if (o != 0) {
                o = (Cmd *)func_02026b2c(o);
            }
            if (o != 0) {
                o->c2(s, name);
                if (o != 0) {
                    o->c1();
                }
            }
        }
        s->pc += 3;
        break;
    case 0xf8:
        func_02022f34(func_0201123c(func_02011264(), 0), data_020850cc, cmd[1]);
        s->pc += 2;
        return 0;
    case 0xf9:
        if (func_02011320(func_02008444()) == 0x17) {
            p = func_02022f04(func_0201123c(func_02011264(), 0), -1);
            if (p != 0) {
                q = func_020082f8(p, data_020cdfac);
                if (q != 0 && func_02026844(q) == 0) {
                    return 0;
                }
            }
        }
        s->pc++;
        break;
    case 0xfa:
        p = func_02022f04(func_0201123c(func_02011264(), 0), -1);
        if (p != 0) {
            q = func_020082f8(p, data_020cdfac);
            if (q != 0) {
                func_0203ddc4(q);
            }
        }
        s->pc++;
        return 0;
    case 0xfb:
        func_020114dc(func_02008444(), 1);
        s->pc++;
        break;
    case 0xfc:
        func_02011478(func_02008444(), 1);
        s->pc++;
        break;
    case 0xfd:
        s->pc += 2;
        func_02011b04(func_02008444());
        break;
    case 0xfe:
        func_02011cdc(func_02008444(), 1);
        s->pc++;
        break;
    case 0xff:
        func_02011cdc(func_02008444(), 0);
        s->pc++;
        break;
    case 0x100:
        func_02011cc4(func_02008444());
        s->pc++;
        return 0;
    case 0x101:
        func_02011cd0(func_02008444());
        s->pc++;
        return 0;
    case 0x102:
        func_02026854(func_02008444(), 5);
        s->pc++;
        break;
    case 0x103:
        func_02026854(func_02008444(), cmd[1]);
        s->pc += 2;
        break;
    case 0x104:
        func_0202687c(func_02008444());
        s->pc++;
        break;
    case 0x105:
        func_020268b0(func_02008444());
        s->pc++;
        break;
    case 0x106:
        func_020120b0(func_02008444(), cmd[1], 0x100, 0);
        s->pc += 3;
        break;
    case 0x107:
        func_020120b0(func_02008444(), cmd[1], cmd[2], cmd[3]);
        s->pc += 4;
        break;
    case 0x108:
        func_02012110(func_02008444(), cmd[1]);
        s->pc += 2;
        break;
    case 0x109:
        func_0202a080(func_0200847c(), cmd[1], cmd[2]);
        s->pc += 3;
        break;
    case 0x10a:
        func_0202a450(func_0200847c(), cmd[1]);
        s->pc += 2;
        break;
    case 0x10b:
        h = func_0202a0c0(func_0200847c(), cmd[1]);
        if (h != 0) {
            func_020268e0(h, func_02027c28(cmd[2]));
            func_020268ec(h);
        }
        s->pc += 3;
        break;
    case 0x10c:
        h = func_0202a0ec(func_0200847c(), cmd[1], cmd[2], cmd[3]);
        if (h != 0) {
            func_020268e0(h, func_02027c28(cmd[4]));
            func_020268ec(h);
        }
        s->pc += 5;
        break;
    case 0x10d:
        k = (short)cmd[5];
        t = func_02028148(s);
        if (t != 0 && func_02008828(t, data_020cf898) != 0 && k >= 0) {
            base = func_020083a0(t);
            func_0205958c(&tmp, t, k);
            func_02008378(&pos, base, &tmp);
            h = func_0202a114(func_0200847c(), cmd[1], pos.x, pos.y);
            if (h != 0) {
                func_02061c10(h, t, k);
            }
        } else {
            h = func_0202a114(func_0200847c(), cmd[1], (short)cmd[2] << 12, (short)cmd[3] << 12);
            if (h != 0) {
                func_020268fc(h);
                func_020268ec(h);
            }
        }
        if (h != 0) {
            func_020268e0(h, func_02027c28(cmd[4]));
        }
        s->pc += 6;
        break;
    case 0x10e:
        name = func_02027c28(cmd[2]);
        if (name[0] != '*') {
            for (func_0202fbcc(&it, 1); func_0200eac4(&it) != 0; func_0200eabc(&it, 0)) {
                if (func_02008828(func_0200eab4(&it), data_020cfbd4) != 0) {
                    e = func_0200eab8(&it);
                    if (e != 0 && func_02026910(e) != 0 && func_020790c8(name, func_02026910(e)) == 0
                        && func_02026914(e) != 0) {
                        func_0202aba4(func_0200847c(), func_02026920(e), cmd[1]);
                    }
                }
            }
        } else {
            func_0202a16c(func_0200847c(), cmd[1]);
        }
        s->pc += 3;
        break;
    case 0x10f:
        func_0202a90c(func_0200847c(), cmd[1]);
        s->pc += 2;
        break;
    case 0x12e:
        s->pc++;
        break;
    case 0x12f:
        s->pc++;
        break;
    case 0x130:
        s->pc++;
        break;
    case 0x131:
        s->pc++;
        break;
    case 0x132:
        s->pc++;
        break;
    case 0x169:
        data_020abf38 = cmd[1];
        s->pc += 2;
        break;
    case 0x16a:
        data_020abf34 = cmd[1];
        s->pc += 2;
        break;
    }
    return 1;
}
