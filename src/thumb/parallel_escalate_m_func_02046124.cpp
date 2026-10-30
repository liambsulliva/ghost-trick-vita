class Inner02046124 {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
};

class Object02046124 {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    char pad[0x154];
    Inner02046124 *inner;
    int unk15c;
    int handle;
};

extern "C" {
void *func_02008444(void);
int func_02031af4(void *manager, int handle);

void func_02046124(Object02046124 *object)
{
    int handle;

    object->v2c();
    if (object->inner != 0) {
        handle = object->handle;
        if (handle >= 0) {
            if (func_02031af4(func_02008444(), handle) == 0) {
                object->inner->v2c();
            }
        }
    }
}
}
