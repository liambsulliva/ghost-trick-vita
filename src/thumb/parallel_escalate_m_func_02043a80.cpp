class Obj {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5c();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6c();
    virtual void v70();
    virtual void v74();
    virtual void v78();
    virtual void v7c();
    virtual void v80();
    virtual void SetId(int id);
    virtual unsigned short GetId();
};

extern "C" {
extern unsigned char data_020cf75c[];
unsigned char *func_02009900(void);
unsigned int func_0202f64c(void *p);
Obj *func_0202fb48(unsigned char *p, unsigned char *kind, int start);
void func_0202fad4(unsigned char *p, unsigned char *kind);
Obj *func_02056964(unsigned char *kind);
void func_0202f7e8(unsigned char *p, Obj *o, int a, int b);
void func_02056d68(Obj *o, int a, unsigned char b);
unsigned short func_02043b80(Obj *o, int z);

void func_02043a80(unsigned char *p, int on)
{
    Obj *o;

    if (on == 0) {
        if (p[0x30] != 0) {
            func_0202fad4(func_02009900(), data_020cf75c);
        } else {
            func_0202f64c(p + 0x34);
            o = func_0202fb48(func_02009900(), data_020cf75c, -1);
            if (o != 0) {
                o->SetId(*(unsigned short *)(p + 0x2e));
            }
        }
        p[0x30] = 0;
        return;
    }
    o = func_0202fb48(func_02009900(), data_020cf75c, -1);
    if (o == 0) {
        o = func_02056964(data_020cf75c);
        func_0202f7e8(func_02009900(), o, 5, 0);
        if (o != 0) {
            o->SetId(0x130);
            func_02056d68(o, 0, 20);
            p[0x30] = 1;
        }
        return;
    }
    *(unsigned short *)(p + 0x2e) = o->GetId();
    o->SetId(0x130);
    p[0x30] = 0;
    if (func_02043b80(o, 0) == 0x7fff) {
        *(Obj **)(p + 0x34) = func_02056964(data_020cf75c);
        func_0202f7e8(func_02009900(), *(Obj **)(p + 0x34), 5, 0);
        if (*(Obj **)(p + 0x34) != 0) {
            (*(Obj **)(p + 0x34))->SetId(0x120);
            func_02056d68(*(Obj **)(p + 0x34), 0, 20);
        }
    }
}
}
