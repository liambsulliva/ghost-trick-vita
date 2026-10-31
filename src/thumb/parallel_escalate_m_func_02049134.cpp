class Actor {
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
    virtual void v84(int value);
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9c();
    virtual void va0();
    virtual void va4();
    virtual void va8(int a, int b, int c, int d, int e, int f, int g);
};
extern "C" {
extern void *func_02031254(int size);
extern void *func_02042718(void *object);
extern void *func_0200883c(void);
extern void *func_0201a4e8(void *object);
extern void *func_0201e888(void *object, int a, int b, int c, int d);
extern void func_020427b0(void *a, void *b, int c, int d, int e, int f, int g, int h, int i, int j, int k);

void func_02049134(Actor *param_1)
{
    void *p;

    param_1->va8(0x14, 0, 1, 0, 1, -1, 0);
    param_1->v84(0x460);
    p = func_02031254(0x54);
    if (p != 0) {
        p = func_02042718(p);
    }
    *(void **)((char *)param_1 + 0xb8) = p;
    *(void **)((char *)param_1 + 0xbc) = func_0201e888(func_0201a4e8(func_0200883c()), 0, 0x13, 0, 0);
    func_020427b0(*(void **)((char *)param_1 + 0xb8), *(void **)((char *)param_1 + 0xbc), 0, 0, 1, 0, 0xffff, 1, -1, -1, 0x100);
}
}
