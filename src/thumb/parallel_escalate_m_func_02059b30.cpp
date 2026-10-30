class Object02059b30 {
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
} Flags02059b30;

typedef struct {
    char before[0xde];
    Flags02059b30 flags;
    char middle[0x35];
    char sub[0x30];
    int index;
} View02059b30;

extern "C" {
void func_02059b18(char *param_1, int param_2);
unsigned int *func_02008444(void);
int func_02046490(char *object);
void func_02007c40(int param_1, unsigned int param_2);

void func_02059b30(View02059b30 *param_1)
{
    int iVar2;

    if (param_1->flags.b4 != 0) {
        param_1->flags.b4 = 0;
        func_02059b18(param_1->sub, 0);
        if (param_1->index != 0) {
            ((Object02059b30 *)param_1)->slot10();
            iVar2 = func_02046490((char *)func_02008444());
            func_02007c40((int)param_1->sub, iVar2 + param_1->index * 2);
        }
    }
}
}
