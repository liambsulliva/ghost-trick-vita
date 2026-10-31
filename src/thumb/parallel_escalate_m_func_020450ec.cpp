class Obj020450ec {
public:
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual void s3();
    virtual void s4();
    virtual void s5();
    virtual void s6();
    virtual void s7();
    virtual void s8();
    virtual void s9();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59();
    virtual void s60();
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
    virtual void s65();
    virtual void s66();
    virtual void s67();
    virtual void s68();
    virtual int s69(Obj020450ec *a, void *b);
    char pad0[0xdc];
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    char pad1[0x38];
    unsigned char f119;
    unsigned char f11a;
    unsigned char pad2;
    unsigned char f11c;
    char pad3[0xb];
    int f128;
    int f12c;
};

extern "C" {
extern char data_0209b534;
extern char data_0209b618;
extern char data_0209b61c;
extern char data_0209b624;
extern char data_0209b634;
void *func_02008444(void);
int func_02031af4(void *a, int b);
void func_02031b04(void *a, int *b);
int func_0200f330(void *a);
void func_0204476c(void *a);
void func_020113f8(void *a);
void func_02032644(void *a, int b);
int func_020267a0(void *a);
void func_0200da04(void *a, void *b, void *c, void *d);
void func_02010384(void *a, int b);
void func_02010b74(void *a);

void func_020450ec(Obj020450ec *self)
{
    int result;

    if (func_02031af4(func_02008444(), self->f12c) != 0) {
        self->f128 = 0;
        func_02031b04(func_02008444(), &self->f12c);
    }
    if (self->f119 == 0 && func_0200f330(func_02008444()) != 0) {
        return;
    }
    func_0204476c(self);
    result = self->s69(self, &data_0209b534);
    if (self->b2 || self->f119) {
        if (self->f119) {
            func_020113f8(func_02008444());
            func_02032644(func_02008444(), 4);
        }
        if (result == 0) {
            if (self->f119) {
                if (self->f11c == 0 && func_020267a0(func_02008444()) != 0) {
                    return;
                }
                if (self->f11a == 0) {
                    self->s65();
                    return;
                }
                func_0200da04(&data_0209b618, &data_0209b61c, &data_0209b624, &data_0209b634);
                func_02010384(func_02008444(), 0);
                func_02010b74(self);
                return;
            } else {
                func_02010b74(self);
                return;
            }
        }
    }
    if (self->f119) {
        return;
    }
}
}
