class Stream {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void Seek(unsigned int offset,int mode);
    virtual void Read(char *dst,unsigned int size);
};

struct Loader {
    int word0;
    int index;
    unsigned char kind;
    unsigned char direct;
    unsigned char pad[6];
    unsigned int word10;
    int word14;
    unsigned int limit;
    char *data;
    char *bufA;
    char *bufB;
    Stream stream;
};

struct Entry {
    unsigned short size;
    unsigned char rest[14];
};

extern "C" {
extern struct Entry data_02083c7c[];
extern void func_0200e03c(struct Loader *loader,int key);
extern unsigned int func_0200e128(struct Loader *loader,unsigned short value);
extern void func_0200e12c(struct Loader *loader,char *src,char *dst);
extern void func_0200e290(struct Loader *loader,char *buf,unsigned int value);
extern void func_02072fa8(char *buf,int value);
extern void func_0207303c(char *src,char *dst,unsigned int size);

char *func_0200df20(struct Loader *loader,int value,int key,int kind,unsigned char second)
{
    char *buf;
    char *dst;
    unsigned int offset;
    unsigned int size;

    if (loader->index >= 3) {
        return 0;
    }
    func_0200e03c(loader,key);
    if (key == 0) {
        return 0;
    }
    loader->kind = kind;
    offset = func_0200e128(loader,(unsigned short)value);
    buf = loader->bufA;
    if (second != 0) {
        buf = loader->bufB;
    }
    size = data_02083c7c[loader->index].size;
    offset = offset * size;
    dst = buf + 0x180;
    if (loader->limit >= offset + size) {
        if (loader->direct != 0) {
            func_0207303c(loader->data + offset,dst,size);
        }
        else {
            loader->stream.Seek(offset,0);
            loader->stream.Read(dst,data_02083c7c[loader->index].size);
        }
    }
    else {
        func_02072fa8(dst,3);
    }
    func_0200e12c(loader,dst,buf);
    if (loader->index != 2) {
        func_0200e290(loader,buf,loader->word10);
    }
    return buf;
}
}
