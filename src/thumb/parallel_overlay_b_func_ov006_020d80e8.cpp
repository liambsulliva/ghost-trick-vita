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
void func_ov006_020d80e8(unsigned char *p)
{
    func_0200b518(p + 0x28, 1, 0x80000000u, 0x80000000u);
    func_ov006_020d8180(p);
    func_020280a8(p + 0x28, p[0xc0]);
    func_0203fdd8(p + 0x28, p[0xc1]);
    ((Object *)p)->slot10();
}
}
