class Inner0204615c {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
};

class Object0204615c {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    char pad04[12];
    unsigned char state;
    char pad[0x147];
    Inner0204615c *inner;
    int unk15c;
    int handle;
};

extern "C" {
void *func_02008444(void);
int func_02031af4(void *manager, int handle);

void func_0204615c(Object0204615c *object)
{
    int handle;

    if (object->state == 1) {
        object->v28();
        if (object->inner != 0) {
            handle = object->handle;
            if (handle >= 0) {
                if (func_02031af4(func_02008444(), handle) == 0) {
                    object->inner->v28();
                }
            }
        }
    }
}
}
