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

struct Part;

class Owner {
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
    virtual void *v2c(signed char index, unsigned int a, unsigned int b);
    char pad04[0x1c];
    void *items[5];
    void *child34;
    char pad38[4];
    Actor *actor;
    char pad40[0x14];
};

extern "C" {
extern void *func_02009900();
extern void *func_0202f624(int size);
extern void *func_020679b8(void *memory);
extern void func_0202f7e8(void *list, void *actor, int a, int b);
extern void *func_0205693c(void *resource);
extern void func_ov005_020d44f4(void *child);
extern char data_020cf724;

void func_ov005_020d443c(Owner *param_1, unsigned int param_2, unsigned int param_3)
{
    signed char i;
    void *child;

    for (i = 0; i < 5; i++) {
        param_1->items[i] = param_1->v2c(i, param_2, param_3);
    }
    if (param_1->actor == 0) {
        param_1->actor = (Actor *)func_0205693c(&data_020cf724);
        param_1->actor->va8(0x412, 0, 1, 1, 1, -1, 0);
        param_1->actor->v84(0x110);
        func_0202f7e8(func_02009900(), param_1->actor, 7, 0);
    }
    if (param_1->child34 == 0) {
        child = func_0202f624(0xc4);
        if (child != 0) {
            child = func_020679b8(child);
        }
        param_1->child34 = child;
        func_ov005_020d44f4(child);
        func_0202f7e8(func_02009900(), param_1->child34, 7, 0);
    }
    *(unsigned char *)((int)param_1 + 0x51) = (*(unsigned char *)((int)param_1 + 0x51) & ~1) | 1;
}
}
