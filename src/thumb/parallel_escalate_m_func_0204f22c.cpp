class Object0204f22c {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(int x, int z, int flags);
    char pad04[0xc6];
    unsigned short a;
    unsigned short b;
    unsigned short c;
    unsigned short d;
    unsigned short pad;
    int x;
    int y;
    int z;
};

extern "C" {

void func_0204f22c(Object0204f22c *object, int x, int y, int z, unsigned short a, unsigned short b, unsigned short c, unsigned short d)
{
    int px;
    int pz;

    px = (x - 0x80) * 0x1000;
    object->x = px;
    object->y = (0x60 - y) * 0x1000;
    pz = (0x60 - z) * 0x1000;
    object->z = pz;
    object->a = a;
    object->b = b;
    object->c = c;
    object->d = d;
    object->v34(px, pz, 0);
}
}
