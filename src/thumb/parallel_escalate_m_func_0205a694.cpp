class Object {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8(int);
};

extern "C" {
extern char data_0209da4c;
extern char data_0209da50;
extern int func_02008444(...);
extern int func_0203435c(...);
extern int func_0200f330(...);
extern int func_02027ea4(...);
extern void func_020330e0(...);
extern void func_020330c0(...);
extern void func_02011a14(...);
extern void func_02011a28(...);
extern void func_0200ee54(...);
extern void func_0200da04(...);
}

extern "C" void func_0205a694(char *param_1)
{
    if (func_0203435c(func_02008444()) == 0) {
        if (*(unsigned char *)(param_1 + 0x44) == 0) {
            if (func_0200f330(func_02008444()) != 0) {
                if (func_02027ea4(func_02008444()) == 0) {
                    return;
                }
            }
        }
        if (*(unsigned char *)(param_1 + 0x10) == 0) {
            if (*(int *)(param_1 + 0x38) != 0) {
                *(int *)(param_1 + 0x38) = *(int *)(param_1 + 0x38) - 1;
                func_020330e0(func_02008444(),*(int *)(param_1 + 0x30));
                func_020330c0(func_02008444(),*(int *)(param_1 + 0x34));
            }
            if (*(int *)(param_1 + 0x38) == 0) {
                func_02011a14(func_02008444(),*(int *)(param_1 + 0x28));
                func_02011a28(func_02008444(),*(int *)(param_1 + 0x2c));
                *(unsigned char *)(param_1 + 0x10) = 1;
            }
        }
        else {
            func_0200ee54(func_02008444(),0);
            func_0200da04(&data_0209da4c,*(int *)(param_1 + 0x3c),*(int *)(param_1 + 0x40),&data_0209da50);
            ((Object *)param_1)->slot8(0);
        }
    }
}
