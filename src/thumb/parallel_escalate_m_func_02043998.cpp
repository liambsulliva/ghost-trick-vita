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
extern void *func_02009900();
extern void *func_0202f624(int size);
extern void func_0202f7e8(void *list, void *actor, int a, int b);
extern Actor *func_020439fc(void *memory);
extern void func_02043fc0(Actor *actor, int value);

Actor *func_02043998(int param_1, int param_2)
{
    Actor *actor;

    actor = (Actor *)func_0202f624(0xd0);
    if (actor != 0) {
        actor = func_020439fc(actor);
    }
    if (actor != 0) {
        actor->va8(0xab, 0, 1, 0, 0, -1, 0);
        actor->v84(0x110);
        func_02043fc0(actor, param_2);
        func_0202f7e8(func_02009900(), actor, 7, 0);
    }
    return actor;
}
}
