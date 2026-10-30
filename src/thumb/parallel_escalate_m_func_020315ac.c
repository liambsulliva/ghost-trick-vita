struct Bits020315ac {
    unsigned char low : 1;
    unsigned char sub : 1;
    unsigned char rest : 6;
};

struct Frame020315ac {
    unsigned char mode;
    unsigned char count;
    unsigned char shown;
    volatile signed char busy;
    int pad04;
    int frames;
    char pad0c[9];
    struct Bits020315ac bits;
    unsigned short main_color;
    unsigned short sub_color;
};

extern struct Frame020315ac data_020cff60;
extern void func_0206995c(void);
extern void func_0203163c(int left, int right);
extern void func_02074810(void);
extern void func_02075d30(void);
extern void func_02023b28(void);
extern void *func_02009c1c(void);
extern void func_0202f1ec(void *object);
extern void func_02008860(void);
extern void func_0200c61c(void);
extern void func_0201003c(void);
extern void func_02015908(void);
extern void func_0202f280(void *object);
extern void func_02008444(void);
extern void func_0203164c(void);
extern void func_0206f8d4(void);
extern void func_02010b7c(void);
extern void func_0202f12c(void);

void func_020315ac(void)
{
    func_0206995c();
    if (data_020cff60.bits.sub == 0) {
        func_0203163c(1, 0);
    } else {
        func_0203163c(1, 1);
    }
    func_02074810();
    data_020cff60.busy = 1;
    func_02075d30();
    while (data_020cff60.busy != 0) {
    }
    data_020cff60.shown = data_020cff60.count;
    data_020cff60.frames++;
    func_02023b28();
    func_0202f1ec(func_02009c1c());
    func_02008860();
    func_0200c61c();
    func_0201003c();
    func_02015908();
    func_0202f280(func_02009c1c());
    func_02008444();
    func_0203164c();
    *(volatile unsigned short *)0x05000000 = data_020cff60.main_color;
    *(volatile unsigned short *)0x05000400 = data_020cff60.sub_color;
    func_0206f8d4();
    func_02010b7c();
    func_0202f12c();
}
