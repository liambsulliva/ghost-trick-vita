class File0201e664 {
public:
    virtual void slot0();
    virtual void release();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void seek(unsigned int pos, int whence);
    virtual void read(void *dst, unsigned int size);
};

class Stream0201e664 {
public:
    virtual void slot0();
    virtual void release();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void seek(unsigned int pos, int whence);
    virtual void read(void *dst, unsigned int size);

    unsigned int offset;
    unsigned int pad8;
    unsigned int size;
    File0201e664 *file;
};

struct CacheEnt0201e664 {
    unsigned int id : 8;
    unsigned int offset : 24;
    unsigned int size;
};

struct Entry0201e664 {
    unsigned int table;
    unsigned int base;
    int type;
    CacheEnt0201e664 *cache;
    unsigned int cacheCount;
    unsigned int pad14;
};

struct Loader0201e664 {
    File0201e664 *file;
    int pad4;
    unsigned char flag;
    Entry0201e664 entries[1];
};

struct RecA0201e664 {
    unsigned char low : 7;
    unsigned char multi : 1;
    unsigned char len;
    unsigned short off;
};

struct RecB0201e664 {
    unsigned int off : 31;
    unsigned int multi : 1;
    unsigned int size;
};

union Rec0201e664 {
    RecA0201e664 a;
    RecB0201e664 b;
};

extern "C" {
extern void *func_02031254(unsigned int size);
extern void *func_0201e540(void *object);
extern void func_0207303c(const void *src, void *dest, unsigned int size);
extern unsigned char data_020cff60;

Stream0201e664 *func_0201e664(Loader0201e664 *loader, int index, unsigned int id)
{
    RecA0201e664 hdrA[5];
    RecB0201e664 hdrB[5];
    Rec0201e664 rec;
    Stream0201e664 *obj;
    CacheEnt0201e664 *cache;
    CacheEnt0201e664 *slot;
    unsigned char n;

    obj = (Stream0201e664 *)func_02031254(0x14);
    if (obj != 0) {
        obj = (Stream0201e664 *)func_0201e540(obj);
    }
    loader->flag = 0;
    cache = loader->entries[index].cache;
    if (cache != 0) {
        slot = &cache[id % loader->entries[index].cacheCount];
        if (slot->id == id) {
            obj->offset = slot->offset;
            obj->size = slot->size;
            goto done;
        }
    }
    switch (loader->entries[index].type) {
    case 0:
        loader->file->seek(loader->entries[index].table + id * 4, 0);
        loader->file->read(hdrA, 0x14);
        if (hdrA[0].multi) {
            n = data_020cff60;
            if (n >= 1) {
                func_0207303c(&hdrA[n - 1], &rec, 4);
                loader->flag = 1;
                goto copied_a;
            }
        }
        func_0207303c(&hdrA[0], &rec, 4);
    copied_a:
        obj->offset = (rec.a.off << 5) + loader->entries[index].base;
        obj->size = rec.a.len << 5;
        break;
    case 1:
        loader->file->seek(loader->entries[index].table + id * 8, 0);
        loader->file->read(hdrB, 0x28);
        if (hdrB[0].multi) {
            n = data_020cff60;
            if (n >= 1) {
                func_0207303c(&hdrB[n - 1], &rec, 8);
                loader->flag = 1;
                goto copied_b;
            }
        }
        func_0207303c(&hdrB[0], &rec, 8);
    copied_b:
        obj->offset = rec.b.off + loader->entries[index].base;
        obj->size = rec.b.size;
        break;
    }
    cache = loader->entries[index].cache;
    if (cache != 0) {
        slot = &cache[id % loader->entries[index].cacheCount];
        slot->id = id;
        slot->offset = obj->offset;
        slot->size = obj->size;
    }
done:
    if (obj->size == 0) {
        if (obj != 0) {
            obj->release();
        }
        obj = 0;
    } else {
        obj->file = loader->file;
        obj->seek(0, 0);
    }
    return obj;
}
}
