struct State {
    unsigned int before[4];
    int data;
};
extern struct State data_020b18a0;
extern void func_020740d0();
extern unsigned int func_02074588();
extern void func_0200792c();
extern void func_02003b48();
extern void func_02005854();
extern void func_02006598();
extern void func_020074ec();
extern void func_0207679c();
extern void func_02074844();
extern void func_0200793c();
extern void func_020745d8();

void func_02006c0c(void)
{
    struct State *iVar1;
    int iVar2;
    int iVar3;

    iVar1 = &data_020b18a0;
    iVar2 = 0x1bb4;
    iVar3 = 0x15a4;
    do {
        func_020740d0(0);
        func_02074588(iVar1->data + iVar2);
        func_0200792c();
        func_02003b48();
        func_02005854();
        func_02006598();
        func_020074ec();
        func_0207679c(iVar1->data);
        func_02074844(iVar1->data, 0x11e0);
        if ((*(unsigned int *)(iVar1->data + iVar3) & 1) != 0) {
            func_0200793c();
        }
        func_020745d8(iVar1->data + iVar2);
    } while (1);
}
