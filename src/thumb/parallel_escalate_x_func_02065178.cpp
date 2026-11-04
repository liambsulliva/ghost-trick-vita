class Codec {
public:
    virtual void s0();
    virtual void s1();
    virtual int s2(unsigned int len, char *name);
};

struct BankId {
    int head;
    int body;
};

struct Banks {
    unsigned char pad0[0x38];
    int heads[4];
    int bodies[4];
    unsigned int headSize[4];
    unsigned int bodySize[4];
    unsigned char pad78[0xc];
    unsigned int total;
    unsigned char pad88[0x14];
    unsigned char count;
};

extern "C" {
extern const struct BankId data_0209e028[];
extern char data_0209e110[];
extern void *func_0200883c(void);
extern void *func_02008824(void *a);
extern unsigned short *func_0201e888(void *a, int b, unsigned short c, int d, int e);
extern unsigned int func_0200cb5c(void *a);
extern void *func_02007bd8(void);
extern Codec *func_02007b70(void *a);
extern void func_0207303c(void *src, void *dst, unsigned int size);
extern void func_020648a8(int a, unsigned int b);
int func_02065178(struct Banks *p);
}

int func_02065178(struct Banks *p)
{
    const struct BankId *ent;
    unsigned int i;
    int id;
    unsigned short *head;
    unsigned short *body;
    unsigned int n;
    unsigned int pad;
    unsigned int j;

    ent = data_0209e028;
    for (i = 0; i < 4 && (id = ent->head) != 0; i++) {
        head = func_0201e888(func_02008824(func_0200883c()), 2, id, 1, 0);
        p->headSize[i] = func_0200cb5c(func_02008824(func_0200883c()));
        if (p->heads[i] == 0) {
            p->heads[i] = func_02007b70(func_02007bd8())->s2(p->headSize[i] + 0x20, data_0209e110);
        }
        func_0207303c(head, (void *)(p->heads[i] + 0x20), p->headSize[i]);
        func_020648a8(p->heads[i], 0x20);
        p->headSize[i] += 0x20;
        body = func_0201e888(func_02008824(func_0200883c()), 2, ent->body, 1, 0);
        pad = 0;
        n = func_0200cb5c(func_02008824(func_0200883c()));
        if (n & 0x7ff) {
            pad = 0x800 - (n - (n & ~0x7ff));
        }
        p->bodySize[i] = n + pad;
        if (p->bodies[i] == 0) {
            p->bodies[i] = func_02007b70(func_02007bd8())->s2(p->bodySize[i], data_0209e110);
        }
        for (j = 0; j < n >> 1; j++) {
            body[j]++;
        }
        func_0207303c(body, (void *)p->bodies[i], n);
        func_020648a8((int)((char *)p->bodies[i] + n), pad);
        p->count++;
        p->total += n >> 3;
        ent++;
    }
    p->total += 0x180;
    return 1;
}
