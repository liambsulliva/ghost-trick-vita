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
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void slot14();
    virtual void slot15();
    virtual void slot16();
    virtual void slot17();
    virtual void slot18();
    virtual void slot19();
    virtual void slot20();
    virtual void slot21();
    virtual void slot22();
    virtual void slot23();
    virtual void slot24();
    virtual void slot25();
    virtual void slot26();
    virtual void slot27();
    virtual void slot28();
    virtual void slot29();
    virtual void slot30(int value);
};
extern "C" {
extern int func_02037aa4(int kind);
extern void func_020386f4(void *object, signed char value);
unsigned int func_0203ae58(unsigned char *p, int param_2)
{
    int neg;
    if (func_02037aa4(6) == 0) {
        return 0;
    }
    p[0xe4] = 1;
    p[0xe5]++;
    neg = -param_2;
    func_020386f4(*(void **)(p + 0xf4), (signed char)neg);
    *(unsigned short *)(p + 0xda) += (unsigned short)(neg * 0x555);
    ((Object *)p)->slot30(*(unsigned short *)(p + 0xda));
    (*(Object **)(p + 0xf8))->slot30(*(unsigned short *)(p + 0xda));
    p[0xdc] = param_2;
    p[0xf0] = 2;
    return 1;
}
}
