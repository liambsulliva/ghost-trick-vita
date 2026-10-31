extern unsigned char data_020c9d50;
extern unsigned char data_020ced3c;
extern unsigned int func_0200847c(void);
extern unsigned int func_02009900(void);
extern void func_020153d8(int, unsigned char *);
extern void func_0202f7e8(unsigned int, int, int, int);
extern int func_0204ed44(int);
extern int func_0204faf4(int);
extern int func_02056164(int);
extern void func_0202a0c0(unsigned int, int);
extern void func_0205627c(unsigned char *);

void func_020563f8(unsigned char *p)
{
    int iVar1;
    unsigned int uVar2;
    int obj;

    if (*(int *)(p + 0x28) != 0) return;
    if (*(unsigned char *)(p + 0x40) != 0) {
        obj = *(int *)(p + 0x3c);
        if (obj != 0 && func_0204faf4(obj) == 0) *(int *)(p + 0x28) = 5;
        return;
    }
    if (*(int *)(p + 0x34) == 0) return;
    if (func_02056164(*(int *)(p + 0x34)) == 0) return;
    if (*(int *)(p + 0x3c) == 0) {
        iVar1 = func_0204ed44((int)&data_020ced3c);
        *(int *)(p + 0x3c) = iVar1;
        if (iVar1 != 0) {
            func_020153d8(iVar1, p);
            uVar2 = func_02009900();
            func_0202f7e8(uVar2, *(int *)(p + 0x3c), 7, 0);
        } else {
            *(int *)(p + 0x28) = 5;
            return;
        }
    }
    iVar1 = func_0204faf4(*(int *)(p + 0x3c));
    if (iVar1 != 1) return;
    func_0205627c(p);
    if (*(int *)(p + 0x28) != 0) {
        if (*(int *)(p + 0x28) != 5) {
            obj = *(int *)(p + 0x3c);
            (*(void (**)(int))(*(int *)obj + 0xd8))(obj);
        }
    } else {
        obj = *(int *)(p + 0x3c);
        if (obj == 0) return;
        iVar1 = (*(int (**)(int))(*(int *)obj + 0xe0))(obj);
        if (iVar1 == 0 && (*(unsigned int *)((int)&data_020c9d50 + 4) & 0x2000) == 0
            && (*(unsigned int *)((int)&data_020c9d50 + 4) & 0x20) == 0) return;
        uVar2 = func_0200847c();
        func_0202a0c0(uVar2, 3);
        *(unsigned char *)(p + 0x40) = 1;
        obj = *(int *)(p + 0x3c);
        (*(void (**)(int))(*(int *)obj + 0xdc))(obj);
    }
}
