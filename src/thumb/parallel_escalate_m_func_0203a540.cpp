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
    virtual void va8(int a, int b, int c, int d, int e, int f, int g);
    virtual void vac();
    virtual void vb0();
    virtual void vb4();
    virtual void vb8(int a, int b);
};
struct Holder {
    void *methods;
    unsigned char pad004[0x40 - 0x4];
    unsigned short h40;
    unsigned char pad042[0xd4 - 0x42];
    unsigned short hd4;
    unsigned short hd6;
    unsigned short hd8;
    unsigned short hda;
    unsigned char bdc;
    unsigned char bdd;
    unsigned char bde;
    unsigned char pad0df;
    unsigned char be0;
    unsigned char be1;
    unsigned char be2;
    unsigned char be3;
    unsigned char be4;
    unsigned char be5;
    unsigned char pad0e6[2];
    int we8;
    int wec;
    unsigned char bf0;
    unsigned char bf1;
    unsigned char pad0f2[2];
    int wf4;
    Actor *actor;
};
extern "C" {
extern char data_020a6c40;
extern void *func_02038aa4(void *object);
extern void *func_0202f624(int size);
extern Actor *func_0203a66c(void *memory);
extern void func_0205765c(Actor *object, int a, int b, int c, int d);
extern void *func_02009900();
extern void func_0202f7e8(void *list, Actor *actor, int a, int b);

struct Holder *func_0203a540(struct Holder *param_1, int param_2)
{
    Actor *actor;

    func_02038aa4(param_1);
    param_1->methods = &data_020a6c40;
    param_1->hd4 = 0;
    param_1->hd6 = 0;
    param_1->hd8 = param_1->h40;
    param_1->hda = param_1->h40;
    param_1->bdc = 0;
    param_1->bdd = 3;
    param_1->bde = 0;
    param_1->be0 = 0;
    param_1->be1 = 0;
    param_1->be2 = 0;
    param_1->be3 = 0;
    param_1->be4 = 0;
    param_1->be5 = 0;
    param_1->we8 = 0;
    param_1->wec = 0;
    param_1->bf0 = 0;
    param_1->bf1 = 0;
    param_1->wf4 = param_2;
    param_1->actor = 0;
    actor = (Actor *)func_0202f624(0xd0);
    if (actor != 0) {
        actor = func_0203a66c(actor);
    }
    param_1->actor = actor;
    if (actor != 0) {
        actor->va8(0x57, 0, 1, 0, 1, -1, 0);
        param_1->actor->v84(0x420);
        func_0205765c(param_1->actor, 0, 0, 0x80000, 0x80000);
        param_1->actor->vb8(0x80000, 0x80000);
        param_1->actor->v34(0x84000, 0xc000, 0);
        func_0202f7e8(func_02009900(), param_1->actor, 1, 0);
    }
    return param_1;
}
}
