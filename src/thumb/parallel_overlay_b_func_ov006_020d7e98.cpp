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
};
extern "C" {
extern void func_0200b518(void *, int, unsigned int, unsigned int);
extern void func_0200b598(void *, int, int);
extern void func_020280a8(void *, int);
extern void func_0203fdd8(void *, int);
extern void func_ov006_020d8180(void *);
void func_ov006_020d7e98(unsigned char *p)
{
    int value;
    func_0200b518(p + 0x28, 1, 0x80000000u, 0x80000000u);
    value = *(int *)(p + 0xc8);
    if (value != -1) {
        func_0200b598(p + 0x28, value, 15);
    } else {
        func_0200b598(p + 0x28, *(int *)(p + 0xd0), 15);
    }
    func_020280a8(p + 0x28, 3);
    func_0203fdd8(p + 0x28, 2);
    ((Object *)p)->slot10();
}
}
