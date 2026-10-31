class Target02011a3c {
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
    virtual void slot10(int a, int b);
};

struct Header02011a3c {
    int f0;
    int f4;
    int f8;
    unsigned int low : 3;
    unsigned int level : 4;
    unsigned int mid : 4;
    unsigned int active : 1;
    unsigned int high : 20;
    int f10;
    unsigned short f14;
    unsigned short f16;
};

extern "C" {
extern int func_02031254(int size);
extern int func_02020ec4(int obj);
extern char *func_02008444(void);
extern void func_02011578(char *p, int n);
extern void func_02020ecc(int obj, Target02011a3c *target, int kind, char *data, int flag);
extern void func_0200e930(char *p, unsigned char level);
extern void func_02011a14(char *p, unsigned int v);
extern void func_02011a28(char *p, unsigned int v);
extern void func_0200ee80(char *p, unsigned short v);
extern unsigned char *func_0200847c(void);
extern void func_0202a044(unsigned char *p, unsigned short a, unsigned short b, unsigned int c);
extern unsigned char *func_02009900(void);
extern void func_0202f7e8(unsigned char *p, int v, int a, int b);
extern void func_02020ec8(int obj);
extern void func_02031270(int obj);

void func_02011a3c(int param_1, Target02011a3c *target)
{
    int obj;
    char *work;
    unsigned int i;
    int v;

    obj = func_02031254(0x1c);
    if (obj != 0) {
        obj = func_02020ec4(obj);
    }
    if (obj != 0) {
        work = func_02008444();
        func_02011578(work + 0xdfc, 0x20);
        i = 0;
        target->slot10(0, 0);
        func_02020ecc(obj, target, 0xd, work + 4, i);
        if (((Header02011a3c *)work)->active) {
            func_0200e930(func_02008444(), (unsigned char)((Header02011a3c *)work)->level);
            func_02011a14(work, *(unsigned int *)(work + 0xd94));
            func_02011a28(work, *(unsigned int *)(work + 0xd98));
            func_0200ee80(work, *(unsigned short *)(work + 0xd90));
            func_0202a044(func_0200847c(), ((Header02011a3c *)work)->f14, ((Header02011a3c *)work)->f16, i);
            do {
                v = *(int *)(work + 0xdfc + i * 4);
                if (v != 0) {
                    func_0202f7e8(func_02009900(), v, 8, 1);
                }
                i++;
            } while (i < 8);
        }
    }
    if (obj != 0) {
        func_02020ec8(obj);
        func_02031270(obj);
    }
}
}
