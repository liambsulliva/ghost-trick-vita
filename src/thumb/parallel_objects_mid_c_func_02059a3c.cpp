class Object02059a3c {
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

typedef struct {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
} Flags02059a3c;

typedef struct {
    char before[0xde];
    Flags02059a3c flags;
    char middle[0x35];
    char sub[4];
} View02059a3c;

extern "C" {
int func_02029ab4(char *param_1);
void func_02008a98(char *param_1);

void func_02059a3c(View02059a3c *param_1)
{
    if (param_1->flags.b3 == 0 && param_1->flags.b4 == 0) {
        param_1->flags.b3 = 1;
        ((Object02059a3c *)param_1)->slot11();
        if (func_02029ab4(param_1->sub) != 0) {
            func_02008a98(param_1->sub);
        }
    }
}
}
