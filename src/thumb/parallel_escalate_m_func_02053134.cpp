struct Pair {
    int first;
    int second;
};
class Item {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual int v10();
};
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
    virtual int v80(int value);
};
extern "C" {
extern void func_0202fbcc(void *iterator, int kind);
extern int func_0200eac4(void *iterator);
extern Item *func_0200eab4(void *iterator);
extern int func_0200eab8(void *iterator);
extern void func_0200eabc(void *iterator, int value);
extern void func_02053198(int object, struct Pair value);

void func_02053134(Owner *param_1, struct Pair value)
{
    unsigned int iterator[4];

    func_0202fbcc(iterator, 6);
    while (func_0200eac4(iterator) != 0) {
        if (param_1->v80(func_0200eab4(iterator)->v10()) != 0) {
            func_02053198(func_0200eab8(iterator), value);
        }
        func_0200eabc(iterator, 0);
    }
}
}
