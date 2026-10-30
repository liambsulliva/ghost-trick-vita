class File {
public:
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void Close();
    int words[19];
};
extern "C" {
struct Manager {
    void (**table)(struct Manager *manager,int handle);
};
struct Stream {
    void *table;
    unsigned char bytes[5];
    unsigned char shared;
    unsigned char pad[0x12];
    int handle0;
    int handle1;
    int handle2;
    File file;
    File extra;
};
extern char data_020a2f08;
extern void *func_02007bd8(void);
extern struct Manager *func_02007b70(void *system);
extern File *func_0201ef74(File *file);
extern void func_02031270(void *object);
struct Stream *func_0200ddd4(struct Stream *stream)
{
    struct Manager *manager;
    stream->table = &data_020a2f08;
    if (stream->handle0 != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            manager = func_02007b70(func_02007bd8());
            manager->table[4](manager,stream->handle0);
            stream->handle0 = 0;
        }
    }
    if (stream->handle1 != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            manager = func_02007b70(func_02007bd8());
            manager->table[4](manager,stream->handle1);
            stream->handle1 = 0;
        }
    }
    if (stream->handle2 != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            manager = func_02007b70(func_02007bd8());
            manager->table[4](manager,stream->handle2);
            stream->handle2 = 0;
        }
    }
    if (stream->shared == 0) {
        stream->file.Close();
    }
    func_0201ef74(&stream->extra);
    func_0201ef74(&stream->file);
    func_02031270(stream);
    return stream;
}
}
