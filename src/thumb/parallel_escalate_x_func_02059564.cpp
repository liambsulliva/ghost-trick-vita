class Object {
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
    unsigned char pad[0xda];
    unsigned char low : 3;
    unsigned char first : 1;
    unsigned char second : 1;
    unsigned char high : 3;
};

extern "C" void func_02059564(Object *object)
{
    if (object->second == 1) {
        object->s28();
        return;
    }
    if (object->first == 1) {
        object->s26();
    }
}
