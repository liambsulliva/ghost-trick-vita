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
extern "C" {
extern int func_02044d74(Actor *object);
extern int func_02044d78(Actor *object);
extern void func_0205765c(Actor *object, int a, int b, int c, int d);
extern void func_02056cb8(Actor *object);

void func_0204f4d8(Actor *param_1)
{
    unsigned int index;
    int x;
    int y;

    index = ((unsigned char *)param_1)[0xc5];
    if (index != ((unsigned char *)param_1)[0xc6]) {
        param_1->va8(*(unsigned short *)((char *)param_1 + index * 2 + 0xc8), 0, 1, 0, 0, -1, 0);
        ((unsigned char *)param_1)[0xc6] = ((unsigned char *)param_1)[0xc5];
        x = func_02044d74(param_1);
        y = func_02044d78(param_1);
        param_1->vb8(x, y);
        func_0205765c(param_1, 0, 0, x, y);
    }
    func_02056cb8(param_1);
}
}
