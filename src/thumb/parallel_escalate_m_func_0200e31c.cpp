class File {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual int Open(const char *path,int mode);
    virtual void Close();
    virtual void slot6();
    virtual void Read(void *buffer,int size);
    virtual void slot8();
    virtual void slot9();
    virtual int Length();
};
class Allocator {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void *Alloc(int size,const char *name);
};
extern "C" {
struct Resource {
    void *data;
    unsigned short size;
    unsigned char type;
    unsigned char load;
    unsigned char opened;
    unsigned char pad[3];
    File file;
};
extern char data_02083ccc[];
extern char data_02083cdc[];
extern char data_02083cf0[];
extern char data_02083cc8[];
extern void *func_02007bd8(void);
extern Allocator *func_02007b70(void *system);
extern void *func_0201ef58(File *file);
struct Resource *func_0200e31c(struct Resource *res,unsigned char type,unsigned char load)
{
    const char *path;
    res->data = 0;
    res->size = 0;
    res->type = 4;
    res->load = load;
    res->opened = 0;
    func_0201ef58(&res->file);
    switch (type) {
    case 0:
        path = data_02083ccc;
        break;
    case 1:
        path = data_02083cdc;
        break;
    case 2:
        path = data_02083cf0;
        break;
    default:
        return res;
    }
    if (res->file.Open(path,1) == 0) {
        return res;
    }
    res->size = res->file.Length();
    res->type = type;
    res->opened = 1;
    if (res->load != 0) {
        res->data = func_02007b70(func_02007bd8())->Alloc(-res->size,data_02083cc8);
        res->file.Read(res->data,res->size);
        res->file.Close();
    }
    return res;
}
}
