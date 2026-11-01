struct Item {
    char data[0xe];
};

struct Walker {
    char pad0[0x14];
    int f14;
    int f18;
    char pad1c[6];
    unsigned char b22;
    unsigned char b23;
    char pad24[6];
    unsigned short h2a;
    char pad2c[0x14];
    short h40;
    char pad42[4];
    unsigned short h46;
    unsigned short h48;
    unsigned short h4a;
    unsigned short h4c;
    char pad4e[0xa];
    unsigned char b58;
    char pad59[0x1b];
    unsigned short h74;
    unsigned char b76;
    char pad77[9];
    unsigned short h80;
    unsigned short h82;
    unsigned short h84;
    unsigned char b86;
    unsigned char b87;
    char pad88[4];
    unsigned short h8c;
    struct Item items[1];
};

extern void func_0201778c(struct Walker *p);
extern void func_02017714(struct Walker *p, int a);
extern void func_0201b878(struct Item *item);
extern void func_0201b890(struct Walker *p);

void func_0201b928(struct Walker *p)
{
    int saved;
    unsigned int i;
    unsigned short v;

    p->h8c = 0;
    saved = p->f18;
    func_0201778c(p);
    p->h2a = p->h8c;
    p->b23 = 0;
    p->b22 = 1;
    p->h46 = p->h74 << 3;
    p->h48 = p->b76 << 3;
    v = p->h46;
    p->h4a = v;
    p->h4c = v;
    func_02017714(p, 0);
    p->h40 = -6;
    for (i = 0; i < p->h84; i++) {
        func_0201b878(&p->items[i]);
    }
    p->h84 = 0;
    p->b87 = 0;
    p->h80 = 0;
    if (p->b86 == 0) {
        p->h82 = 0xffff;
    }
    p->b86 = 0;
    p->b58 = 1;
    func_0201b890(p);
    p->f14 = saved;
    p->f18 = saved;
}
