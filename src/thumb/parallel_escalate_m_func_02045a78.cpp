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
    virtual void v30(void *a);
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
    virtual void v84(int a);
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9c();
    virtual void va0();
    virtual void va4();
    virtual void va8();
    virtual void vac();
    virtual void vb0();
    virtual void vb4();
    virtual void vb8();
    virtual void vbc();
    virtual void vc0();
    virtual void vc4();
    virtual void vc8();
    virtual void vcc();
    virtual void vd0();
    virtual void vd4(unsigned char a);
    virtual void vd8();
    virtual void vdc();
    virtual void ve0();
    virtual void ve4();
    virtual void ve8();
    virtual void vec();
    virtual void vf0(void *a);
    virtual void vf4();
    virtual void vf8();
    virtual void vfc();
    virtual void v100();
    virtual void v104(int a);
    virtual void v108();
    virtual void v10c();
    virtual void v110();
    virtual void v114();
    virtual void v118();
    virtual void v11c();
    virtual void v120();
    virtual void v124();
    virtual void v128();
    virtual void v12c();
    virtual void v130();
    virtual void v134();
    virtual void v138();
    virtual void v13c();
    virtual void v140();
    virtual void v144();
    virtual void v148();
    virtual void v14c();
    virtual void v150();
    virtual void v154();
    virtual void v158(int a);
};

struct Owner {
    unsigned char pad000[0x158];
    Obj *obj;
    int w15c;
    void *w160;
};

extern "C" {

extern unsigned char data_020cfe80[];
extern Obj *func_02065b30(void *a);
extern void func_020153d8(Obj *a, struct Owner *b);
extern void func_02058a80(void *a, int b);
extern void func_02008398(void *a, int b, int c, int d);
extern void *func_02008444(void);
extern void *func_02031b44(void *a);
extern void *func_02009900(void);
extern void func_0202f7e8(void *a, void *b, unsigned int c, int d);
extern void *func_0200847c(void);
extern void func_0202a0c0(void *a, int b);

void func_02045a78(struct Owner *param_1, int param_2, int param_3, int param_4, unsigned char param_5, int param_6, int param_7, unsigned int param_8)
{
    unsigned char pos_b[12];
    unsigned char pos_a[12];
    void *value;

    param_1->obj = func_02065b30(data_020cfe80);
    if (param_1->obj == 0) {
        return;
    }
    param_1->obj->v104(param_2);
    param_1->obj->v158(1);
    param_1->obj->v84(param_3);
    func_020153d8(param_1->obj, param_1);
    if (param_4 != 0) {
        func_02058a80(pos_b, param_4);
        param_1->obj->v30(pos_b);
    } else {
        func_02008398(pos_a, param_6, param_7, 0);
        param_1->obj->v30(pos_a);
    }
    param_1->obj->vd4(param_5);
    value = func_02031b44(func_02008444());
    param_1->w160 = value;
    param_1->obj->vf0(value);
    func_0202f7e8(func_02009900(), param_1->obj, param_8, 0);
    func_0202a0c0(func_0200847c(), 6);
}
}
