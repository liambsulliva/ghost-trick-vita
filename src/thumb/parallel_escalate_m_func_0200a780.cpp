class Obj
{
public:
    virtual int s0();
    virtual int s1();
    virtual int s2();
    virtual unsigned int s3(unsigned int a);
    virtual int s4();
    virtual int s5();
    virtual void s6(unsigned int a, int b);
    virtual unsigned int s7(unsigned int a, unsigned int b);
};

struct Holder
{
    Obj *obj;
    unsigned char tag;
};

extern "C" {
unsigned char *func_02007bd8(void);
Obj *func_02007b70(unsigned char *p);
unsigned char *func_02009c1c(void);
void func_0202f1bc(unsigned char *p, unsigned int a, unsigned int b, unsigned int c, unsigned int d);
}

extern "C" unsigned int func_0200a780(Holder *param_1)
{
    unsigned char *puVar1;
    Obj *piVar2;
    unsigned int uVar3;
    unsigned char *puVar4;
    if (param_1->obj != 0) {
        puVar1 = func_02007bd8();
        piVar2 = func_02007b70(puVar1);
        uVar3 = piVar2->s3(0x200);
        if (uVar3 != 0) {
            param_1->obj->s6(0x200, 0);
            param_1->obj->s7(uVar3, 0x200);
            puVar4 = func_02009c1c();
            func_0202f1bc(puVar4, uVar3, (unsigned int)param_1->tag * 0x200 + 0x6000, 0x200, 0x10);
        }
        return uVar3;
    }
    return 0;
}
