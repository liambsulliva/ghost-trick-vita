typedef struct Pad72b0 {
    char pad0[0x12];
    short hold;
} Pad72b0;

class Scene72b0 {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    unsigned char state;
    unsigned char timer;
    char pad6[0x1e];
    unsigned char found;
};

extern "C" {
extern char data_020cfd28[];
extern char data_020cfd0c[];
extern char data_020e45f8[];
extern unsigned int data_020c9e48;
extern Pad72b0 data_020c9e88;
extern void *func_02009900(void);
extern void *func_0202fb48(void *a, char *name, int b);
extern int func_02049ab8(void *item);
extern void *func_0202f25c(void);
extern int func_02061bd4(void *a);
extern void func_02023bb4(char *a);
extern int func_0201572c(void);
extern void func_0202fad4(void *a, char *name);
extern int func_0202f24c(void *a);
extern void func_ov004_020d5958(void *a);
extern void func_02022f6c(Scene72b0 *p);
void func_ov004_020d72b0(Scene72b0 *p);
}

void func_ov004_020d72b0(Scene72b0 *p)
{
    void *item;

    switch (p->state) {
    case 0:
        item = func_0202fb48(func_02009900(), data_020cfd28, -1);
        if (item == 0 || (item != 0 && func_02049ab8(item) != 0)) {
            data_020c9e48 &= 0xffffe7ff;
            p->s11();
            p->found = item != 0;
            p->state++;
        }
        break;
    case 1:
        if (func_02061bd4(func_0202f25c()) == 0) {
            break;
        }
        func_02023bb4(data_020e45f8);
        if (func_0201572c() == 0 && data_020c9e88.hold != 0) {
            data_020c9e88.hold = 0;
        }
        if (p->found == 0) {
            func_0202fad4(func_02009900(), data_020cfd0c);
            p->state = 99;
        } else {
            p->state++;
            p->timer = 6;
        }
        break;
    case 2:
        p->timer--;
        if (p->timer == 0) {
            func_0202fad4(func_02009900(), data_020cfd0c);
            p->state = 99;
        }
        break;
    case 99:
        if (func_0202f24c(func_0202f25c()) == 0) {
            func_ov004_020d5958(func_0202f25c());
            p->s12();
            p->state++;
        }
        break;
    default:
        func_02022f6c(p);
        break;
    }
}
