class Obj064 {
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
    virtual void slot15(int);
    virtual void slot16();
    virtual void slot17();
    virtual void slot18(int);
    virtual void slot19(int, int);
    virtual void slot20();
    virtual void slot21(int, int);
    virtual void slot22();
    char pad04[0xc];
    unsigned char state;
    char pad11[0x17];
    int idx;
    int timer;
    int f30;
    int f34;
    int f38;
    int f3c;
    char pad40[0x14];
    unsigned int pos;
    int f58;
    unsigned int limit;
    int f60;
    int f64;
    int a68;
    int b6c;
    int c70;
    int c74;
    char f78;
    unsigned char f79;
    unsigned char f7a;
};

extern "C" {
extern unsigned char data_020c9e48[];
extern unsigned char data_020c9e88[];
extern const unsigned char data_0209e011[];
extern void func_02071dd0(void);
extern void func_02071960(int value);

void func_02064648(Obj064 *p)
{
    unsigned int n;
    int idx;

    *(unsigned int *)data_020c9e48 = 0x00210810;
    *(unsigned short *)(data_020c9e48 + 10) = 0x8011;
    func_02071dd0();
    func_02071960(4);
    *(unsigned int *)(data_020c9e48 + 0x54) = 0x00210210;
    *(unsigned short *)(data_020c9e88 + 0x1a) = 0x8205;
    p->slot15(0);
    p->slot15(1);
    p->slot18(0);
    p->c70 = p->c70 + 1;
    p->slot18(1);
    p->c74 = p->c74 + 1;
    p->limit = 0xc0;
    idx = p->idx;
    n = data_0209e011[idx * 2];
    if (n > 2) {
        p->limit += (n - 2) * 0xc0;
    }
    if (p->idx == 0) {
        p->limit = 0;
    }
    if (p->idx == 9) {
        p->f7a = 1;
    }
    p->slot21(0, 0);
    p->slot21(-0xc0000, 1);
    p->timer = 0x8c;
    if (p->f7a != 0) {
        p->limit = 0x780;
        p->slot21(-0xc0000, 0);
        p->slot21(-0x180000, 1);
        p->slot15(1);
        p->c74 = p->c74 - 1;
        p->timer = 0x3c;
    }
}
}
