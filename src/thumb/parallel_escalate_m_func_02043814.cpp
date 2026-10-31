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
    virtual void v34(int a, int b, int c);
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
    virtual void vb8(int a, int b);
    virtual void vbc();
};

extern "C" {
extern Obj *func_0202f624(int size);
extern Obj *func_02043cb4(Obj *obj);
extern int func_0205765c(Obj *obj, int a, int b, int c, int d);
extern int func_02043f30(Obj *obj, int a, int b);
extern int func_02043fa8(Obj *obj, short a);
extern unsigned char *func_02009900(void);
extern int func_0202f7e8(unsigned char *a, Obj *obj, int b, int c);

Obj *func_02043814(int param_1, int param_2, int param_3)
{
    Obj *obj;

    obj = func_0202f624(0x12c);
    if (obj != 0) {
        obj = func_02043cb4(obj);
    }
    if (obj != 0) {
        obj->v84(0x120);
        func_0205765c(obj, 0, 0, 0x100000, 0x40000);
        obj->vb8(0x100000, 0x40000);
        obj->v34(0, (param_3 * 0x40 - 0x40) * 0x1000, 0);
        func_02043f30(obj, param_2, *(unsigned char *)(param_1 + 0x2d));
        func_02043fa8(obj, (short)param_3);
        func_0202f7e8(func_02009900(), obj, 7, 0);
    }
    return obj;
}
}
