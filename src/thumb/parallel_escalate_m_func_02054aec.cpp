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
    virtual void v28(void);
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
    virtual void v84();
    virtual void v88();
    virtual void v8c();
    virtual void v90();
    virtual void v94();
    virtual void v98();
    virtual void v9c();
    virtual void va0();
    virtual void va4();
    virtual void va8(int a, int b, int c, int d, int e, int f, int g);
    virtual void vac();
    virtual void vb0();
    virtual void vb4();
    virtual void vb8(int a, int b);
};
struct Owner {
    unsigned char pad00[0x28];
    Actor *a;
    Actor *b;
    Actor *c;
};
extern "C" {
extern void func_0205765c(Actor *object, int a, int b, int c, int d);

void func_02054aec(struct Owner *param_1)
{
    param_1->a->va8(0x17, 0, 1, 0, 1, -1, 0);
    func_0205765c(param_1->a, 0x10000, 0, 0x1f000, 0x30000);
    param_1->a->vb8(0xe0000, 0x30000);
    param_1->a->v28();
    param_1->b->va8(0x17, 0, 1, 0, 1, -1, 0);
    func_0205765c(param_1->b, 0, 0, 0x10000, 0x30000);
    param_1->b->vb8(0x10000, 0x30000);
    param_1->b->v28();
    param_1->c->va8(0x17, 0, 1, 0, 1, -1, 0);
    func_0205765c(param_1->c, 0xf000, 0, 0, 0x30000);
    param_1->c->vb8(0x10000, 0x30000);
    param_1->c->v28();
}
}
