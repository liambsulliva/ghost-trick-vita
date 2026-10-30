class Object02059454 {
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
} Flags02059454;

typedef struct {
    char before[0x28];
    char sub[0xb6];
    Flags02059454 flags;
} View02059454;

extern "C" {
void func_0200bbd0(char *param_1);

void func_02059454(View02059454 *param_1)
{
    if (param_1->flags.b3 != 0) {
        return;
    }
    if (param_1->flags.b4 != 0) {
        return;
    }
    param_1->flags.b3 = 1;
    ((Object02059454 *)param_1)->slot11();
    func_0200bbd0(param_1->sub);
}
}
