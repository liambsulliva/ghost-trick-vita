class Object02033998_0 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual int slot4();
};

extern "C" {
typedef int code(...);
extern int func_0201123c(...);
extern int func_02011264(...);
extern int func_02022f04(...);

unsigned int func_02033998(void)

{
    unsigned int uVar1;
    int *piVar2;

    uVar1 = func_02011264();
    uVar1 = func_0201123c(uVar1,0);
    piVar2 = (int *)func_02022f04(uVar1,0xffffffff);
    if (piVar2 == (int *)0x0) {
        return ((unsigned int)0x020cd77cu);
    }
    uVar1 = ((Object02033998_0 *)piVar2)->slot4();
    return uVar1;
}
}
