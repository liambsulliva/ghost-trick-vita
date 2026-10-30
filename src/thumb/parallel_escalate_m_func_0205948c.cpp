class Object0205948c {
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
} Flags0205948c;

typedef struct {
    char before[0x28];
    char sub[0xb6];
    Flags0205948c flags;
} View0205948c;

extern "C" {
int func_0200ba28(char *object);
void func_0200b598(char *object, int value, int mode);

void func_0205948c(View0205948c *param_1)
{
    int iVar1;

    if (param_1->flags.b3 == 1) {
        param_1->flags.b3 = 0;
        ((Object0205948c *)param_1)->slot10();
        iVar1 = func_0200ba28(param_1->sub);
        func_0200b598(param_1->sub, iVar1, 3);
    }
}
}
