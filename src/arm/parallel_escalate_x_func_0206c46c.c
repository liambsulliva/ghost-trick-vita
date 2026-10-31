typedef struct {
    void *arc;
    unsigned int file_id;
} FileId;

typedef struct {
    int pos;
    const char *path;
    unsigned int file_id;
    int find_directory;
} FindArg;

typedef struct {
    unsigned int link[2];
    void *arc;
    unsigned int stat;
    void *arg;
    unsigned int rest[13];
} File;

extern void *func_0206bcd4(const char *path, int *pos, char *buf);
extern void func_0206c440(File *file);
extern int func_0206b9dc(File *file, int command, int block);

int func_0206c46c(FileId *id, const char *path)
{
    int ret = 0;
    char buf[280];
    int pos = 0;
    void *arc = func_0206bcd4(path, &pos, buf);

    if (arc != 0) {
        File file[1];
        FindArg arg;

        func_0206c440(file);
        file->arc = arc;
        file->arg = &arg;
        arg.pos = pos;
        arg.path = buf;
        arg.find_directory = 0;
        if (func_0206b9dc(file, 4, 1)) {
            id->arc = arc;
            id->file_id = arg.file_id;
            ret = 1;
        }
    }
    return ret;
}
