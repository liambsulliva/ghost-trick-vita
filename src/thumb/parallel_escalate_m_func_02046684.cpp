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
    Actor *items[2];
};
extern "C" {
extern void func_0205765c(Actor *object, int a, int b, int c, int d);
extern void *func_02009900(void);
extern void func_0202f7e8(void *list, Actor *object, int a, int b);

void func_02046684(struct Owner *param_1)
{
    unsigned int i;

    for (i = 0; i < 2; i++) {
        param_1->items[i]->va8(0x12, 0, 1, 1, 0, -1, 0);
        param_1->items[i]->vb8(0x110000, 0x10000);
        func_0205765c(param_1->items[i], 0, 0, 0x110000, 0x10000);
        func_0202f7e8(func_02009900(), param_1->items[i], 7, 0);
    }
}
}
