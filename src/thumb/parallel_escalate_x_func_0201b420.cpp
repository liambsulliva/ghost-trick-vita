typedef struct Point1b420 {
    unsigned short x;
    unsigned short y;
} Point1b420;

typedef struct Line1b420 {
    unsigned short c[3];
} Line1b420;

typedef struct Entry1b420 {
    unsigned char pad[0xe];
} Entry1b420;

class Text1b420 {
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
    virtual void s69();
    virtual void s70();
    virtual void s71();
    virtual void s72();
    virtual void s73();
    virtual void s74();
    virtual void s75();
    virtual void s76();
    virtual void s77();
    virtual void s78();
    virtual void s79();
    virtual void s80();
    virtual void s81();
    virtual void s82();
    virtual void s83(int a);
    char pad4[0x10];
    unsigned short *text;
    char pad18[0x6c];
    unsigned short cursor;
    char pad86;
    unsigned char boxed;
    char pad88[6];
    Entry1b420 entries[1];
};

extern "C" {
extern const Line1b420 data_02084a84;
extern Point1b420 func_0201b4c4(Entry1b420 *entry);
extern int func_0201708c(Text1b420 *t);
int func_0201b420(Text1b420 *t);
}

int func_0201b420(Text1b420 *t)
{
    if (t->boxed != 0) {
        Line1b420 line = data_02084a84;
        Point1b420 a = func_0201b4c4(&t->entries[t->cursor]);
        line.c[1] = a.x;
        Point1b420 b = func_0201b4c4(&t->entries[t->cursor]);
        line.c[2] = b.y;
        t->s83(0);
        func_0201708c(t);
        unsigned short *saved = t->text;
        t->text = line.c;
        t->s82();
        t->text = saved;
    } else {
        func_0201708c(t);
    }
    return 0;
}
