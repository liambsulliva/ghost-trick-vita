class Stream0201e7f8 {
public:
    virtual void slot0();
    virtual void release();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void read(void *dst, unsigned int size);
    virtual void slot8();
    virtual void slot9();
    virtual unsigned int info();
};

struct Loader0201e7f8 {
    int pad0;
    unsigned int size;
};

extern "C" {
extern Stream0201e7f8 *func_0201e664(Loader0201e7f8 *loader, int index, unsigned int id);
extern void *func_02007bd8(void);
extern void *func_020086ac(void *heap);
extern unsigned int func_0201e880(void *buf);
extern void func_02073174(void *buf, unsigned char *dst);
extern void func_02074860(unsigned char *start, unsigned int size);

unsigned char *func_0201e7f8(Loader0201e7f8 *loader, int index, unsigned int id, unsigned char *dst)
{
    Stream0201e7f8 *stream;
    unsigned char *ret;
    unsigned int value;
    void *buf;

    stream = func_0201e664(loader, index, id);
    ret = 0;
    loader->size = 0;
    if (stream != 0) {
        value = stream->info();
        if (value & 0x80000000) {
            buf = func_020086ac(func_02007bd8());
            stream->read(buf, value & 0x000fffff);
            value = func_0201e880(buf);
            ret = dst;
            func_02073174(buf, ret);
        } else {
            ret = dst;
            stream->read(ret, value);
        }
        if (ret != 0) {
            func_02074860(ret, value);
            loader->size = value;
        }
    }
    if (stream != 0) {
        stream->release();
    }
    return ret;
}
}
