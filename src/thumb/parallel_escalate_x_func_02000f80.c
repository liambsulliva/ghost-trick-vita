typedef struct Handle00f80 {
    unsigned int id : 24;
    unsigned int flags : 8;
} Handle00f80;

typedef struct File00f80 {
    int size;
    void *data;
    int f8;
    int fc;
} File00f80;

typedef struct Table00f80 {
    char header[0x10];
    File00f80 files[1];
} Table00f80;

typedef struct Archive00f80 {
    char pad[0x90];
    Table00f80 *table;
} Archive00f80;

typedef struct Bank00f80 {
    char pad[0x15b4];
    void *buffer;
} Bank00f80;

typedef struct Game00f80 {
    char pad[0x10];
    Bank00f80 *bank;
} Game00f80;

extern Game00f80 data_020b18a0;

extern int func_02001020(Archive00f80 *archive, Handle00f80 *handle, void *data);
extern int func_02000e00(Archive00f80 *archive, Handle00f80 *handle);
extern int func_02000dc4(unsigned int id);
extern int func_02000d64(Archive00f80 *archive, unsigned int index, void *dest, unsigned int length, unsigned int offset);
extern int func_02000e28(void *buffer, void *data, int arg);

int func_02000f80(Archive00f80 *archive, unsigned int *key, Handle00f80 *handle, int arg)
{
    int result;
    if (archive->table->files[handle->id].size == -1) {
        func_02001020(archive, handle, archive->table->files[handle->id].data);
        result = func_02000e00(archive, handle);
        if (result == -3 || result == -2) {
            return result;
        }
        archive->table->files[handle->id].size = func_02000dc4(handle->id);
    }
    result = func_02000d64(archive, *key, data_020b18a0.bank->buffer, archive->table->files[*key].size, 0);
    if (result == -3 || result == -2) {
        return result;
    }
    return func_02000e28(data_020b18a0.bank->buffer, archive->table->files[handle->id].data, arg);
}
