class Object02065524_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2(int argument0);
};

class Object02065524_1 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
};

class Object02065524_2 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
};

extern "C" {
typedef int code(...);
extern int func_02064854(...);
extern int func_020654dc(...);

void func_02065524(int *param_1,int param_2,int param_3)

{
    int iVar1;

    iVar1 = func_020654dc();
    if (iVar1 == 0) {
        param_1[6] = param_2;
        param_1[7] = param_3;
        iVar1 = func_02064854(param_2);
        ((Object02065524_0 *)param_1)->slot2((int)(iVar1));
        if (-0xc0 < iVar1) {
            ((Object02065524_1 *)param_1)->slot3();
            return;
        }
        ((Object02065524_2 *)param_1)->slot4();
    }
    return;
}
}
