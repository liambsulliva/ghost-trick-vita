struct State {
    unsigned char pad[0x15];
    unsigned char flags;
};

extern "C" {
extern State data_020cff60;
void func_0203052c(void);
void func_020314f0(void);
void *func_020312e4(void);
void func_02008c2c(void *object);
void *func_02008eb0(void);
unsigned int func_01ff80b8(void *object);
void func_0202286c(void);
void func_020317c8(void);
void func_0203171c(int value);
void *func_02010b7c(void);
void func_0202f07c(void *object);
void *func_02008444(void);
void func_0200fd14(void *object);
void *func_02011264(void);
void *func_0201123c(void *object, int index);
void func_02022ff4(void *object);
void func_02042434(void);
void func_0202b41c(void);
void *func_0201003c(void);
unsigned int func_020158ec(void *object);
void *func_0202970c(void);
void func_0200ad58(void *object);
void *func_02009900(void);
void func_0202f818(void *object, int kind, unsigned int priority);
void func_02012450(void *object);
void func_0200f1e0(void *object);
void func_0202f9ac(void);
void *func_02008860(void);
void func_0200c5e4(void *object);
void func_02042464(void);
void *func_0200eb78(void);
void func_02022b34(void *object);
void *func_02009c1c(void);
void func_0202f4c4(void *object);
void *func_0200847c(void);
void func_0202a01c(void *object);
void func_020312d8(void *object);
void func_020315ac(void);
}

namespace dsd {
void main(void)
{
    func_0203052c();
    func_020314f0();
    func_02008c2c(func_020312e4());
    for (;;) {
        func_01ff80b8(func_02008eb0());
        func_0202286c();
        func_020317c8();
        func_0203171c(0);
        func_0202f07c(func_02010b7c());
        func_0200fd14(func_02008444());
        func_02022ff4(func_0201123c(func_02011264(), 0));
        func_02022ff4(func_0201123c(func_02011264(), 1));
        func_02042434();
        func_0202b41c();
        func_020158ec(func_0201003c());
        func_0200ad58(func_0202970c());
        func_0202f818(func_02009900(), 0, 0xd);
        func_02012450(func_02008444());
        func_0200f1e0(func_02008444());
        if ((((unsigned int)data_020cff60.flags << 25) >> 31) == 0) {
            func_02009900();
            func_0202f9ac();
            func_0200c5e4(func_02008860());
            func_02042464();
            func_02022b34(func_0200eb78());
            func_0202f4c4(func_02009c1c());
        }
        func_0202a01c(func_0200847c());
        func_020312d8(func_02009900());
        func_020315ac();
    }
}
}
