class Codec {
public:
    virtual void s0();
    virtual void s1();
    virtual int s2(unsigned int len, char *name);
};

struct Voice {
    unsigned char pad0[8];
    int pos;
    unsigned char padc[4];
    int start;
    int alt;
    unsigned char pad18[0xc];
    unsigned char loud;
    unsigned char pad25[0x14];
    unsigned char state;
    unsigned char pad3a[0xa];
    unsigned short *buf;
    unsigned char pad48[4];
    unsigned int len;
    int out;
};

extern "C" {
extern char data_0209b4c4[];
extern char data_020ce620[];
extern void *func_0200eb78(void);
extern int func_02022c48(void *a, unsigned int b);
extern unsigned short func_02022d04(void *a, unsigned short b, int c);
extern int func_02022c64(void *a, int b);
extern void *func_02009c1c(void);
extern void func_0202f1bc(void *a, void *b, int c, int d, int e);
extern void *func_02007bd8(void);
extern Codec *func_02007b70(void *a);
extern void func_0200d114(char *a, unsigned short *b, int c, unsigned int d, int e);
void func_020428f0(Voice *p, unsigned short *buf, unsigned int len, int param_4, int param_5, int param_6, int param_7, int param_8, int param_9);
}

void func_020428f0(Voice *p, unsigned short *buf, unsigned int len, int param_4, int param_5, int param_6, int param_7, int param_8, int param_9)
{
    unsigned int n;
    int pos;
    unsigned int i;
    int alt;
    int size;

    p->buf = buf;
    p->len = len;
    p->start = param_8;
    p->loud = param_7;
    if (p->state != 0xff) {
        if (len == 0) {
            p->pos = -1;
            p->alt = -1;
            p->state = 0xff;
        } else {
            if (p->state == 0) {
                if (param_8 == -1) {
                    p->pos = func_02022c48(func_0200eb78(), len);
                } else {
                    p->pos = param_8;
                }
            }
            pos = p->pos;
            if (pos != -1) {
                if (param_9 != 0x100) {
                    n = len >> 1;
                    for (i = 0; i < n; i++) {
                        buf[i] = func_02022d04(func_0200eb78(), buf[i], param_9);
                    }
                }
                p->state = 0xff;
                if (param_7 != 0) {
                    func_0202f1bc(func_02009c1c(), buf, pos, len, 1);
                } else {
                    func_0202f1bc(func_02009c1c(), buf, pos, len, 2);
                }
                if (param_5 != 0) {
                    p->alt = func_02022c48(func_0200eb78(), len);
                    p->out = func_02007b70(func_02007bd8())->s2(len, data_0209b4c4);
                    alt = p->alt;
                    size = func_02022c64(func_0200eb78(), alt);
                    func_0200d114(data_020ce620, buf, p->out, len, 0);
                    func_0202f1bc(func_02009c1c(), (void *)p->out, alt, size, 2);
                } else {
                    p->alt = -1;
                }
            }
        }
    }
}
