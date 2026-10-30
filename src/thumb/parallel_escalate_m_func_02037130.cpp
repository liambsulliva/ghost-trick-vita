class Owner {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual int slot6(unsigned int index);
};

extern "C" {
extern char *func_02008444(void);
extern Owner *func_0201ff10(char *object, int index);
extern int func_0201f944(Owner *object, unsigned int index);

unsigned int func_02037130(int param_1)
{
    char *base;
    unsigned int index;
    Owner *owner;

    base = func_02008444();
    index = 0;
    owner = func_0201ff10(base, 0);
    if (owner != 0) {
        for (; index < 0x18; index++) {
            if (func_0201f944(owner, index) >= 0 && param_1 == owner->slot6(index)) {
                return index;
            }
        }
    }
    return 0xffffffff;
}
}
