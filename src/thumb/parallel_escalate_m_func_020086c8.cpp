class Reader {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void Seek(int offset,int origin);
    virtual void Read(void *buffer,int size);
};
extern "C" {
struct Manager {
    void *(**table)(struct Manager *manager,int size,void *name);
};
struct Entry {
    int offset;
    int size;
};
struct Archive {
    int words[6];
    struct Entry *entries;
    Reader *reader;
};
extern char data_02083a30;
extern void *func_02007bd8(void);
extern struct Manager *func_02007b70(void *system);
extern void *func_020086ac(void *system);
extern int func_020086c0(void *buffer);
extern void func_02074860(void *buffer,int size);
extern void func_02073174(void *buffer,void *data);
void *func_020086c8(struct Archive *archive,int index)
{
    void *result;
    int size;
    void *buffer;
    struct Manager *manager;
    void *data;
    result = 0;
    size = archive->entries[index].size;
    archive->reader->Seek(archive->entries[index].offset,0);
    buffer = func_020086ac(func_02007bd8());
    archive->reader->Read(buffer,size);
    size = func_020086c0(buffer);
    manager = func_02007b70(func_02007bd8());
    data = manager->table[2](manager,size,&data_02083a30);
    if (data != 0) {
        func_02074860(buffer,size);
        func_02073174(buffer,data);
        return data;
    }
    return result;
}
}
