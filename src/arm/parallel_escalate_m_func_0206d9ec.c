typedef struct {
    void *arc;
    unsigned short own_id;
    unsigned short index;
    unsigned int pos;
} DirPos;

typedef struct {
    void *arc;
    unsigned int file_id;
} FileId;

typedef union {
    FileId file;
    DirPos dir;
} FindResult;

typedef struct {
    unsigned int link[2];
    void *arc;
    unsigned int pad[9];
    DirPos pos;
    const char *path;
    int find_directory;
    FindResult *result;
} File;

extern void func_0206c440(File *file);
extern int func_0206d714(File *file, int command, int block);

int func_0206d9ec(void *arc, unsigned int base_id, const char *path, unsigned int *out, int is_dir)
{
    FindResult result;
    File file;
    int err;

    func_0206c440(&file);
    file.arc = arc;
    file.pos.arc = arc;
    file.pos.own_id = (unsigned short)base_id;
    file.pos.index = 0;
    file.pos.pos = 0;
    file.path = path;
    file.find_directory = is_dir;
    file.result = &result;
    err = func_0206d714(&file, 4, 1);
    if (err != 0) {
        return err;
    }
    if (is_dir) {
        *out = result.dir.own_id;
    } else {
        *out = result.file.file_id;
    }
    return err;
}
