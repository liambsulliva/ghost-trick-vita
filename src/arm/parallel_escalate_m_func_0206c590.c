typedef struct FSArchive FSArchive;

typedef struct FSFile {
    struct FSFile *next;
    void *userdata;
    FSArchive *arc;
    unsigned int stat;
    void *argument;
    int error;
} FSFile;

typedef struct FSArgumentForOpenFile {
    unsigned int baseid;
    const signed char *relpath;
    unsigned int mode;
} FSArgumentForOpenFile;

extern FSArchive *func_0206bcd4(const signed char *path, unsigned int *baseid, signed char *relpath);
extern void func_0206c440(FSFile *file);
extern int func_0206b9dc(FSFile *file, int command, int blocking);

int func_0206c590(FSFile *file, const signed char *path, unsigned int mode)
{
    int retval = 0;
    {
        signed char relpath[280];
        unsigned int baseid = 0;
        FSArchive *arc = func_0206bcd4(path, &baseid, relpath);

        if (arc) {
            FSArgumentForOpenFile arg;

            func_0206c440(file);
            file->arc = arc;
            file->argument = &arg;
            arg.baseid = baseid;
            arg.relpath = relpath;
            arg.mode = mode;
            if (func_0206b9dc(file, 13, 1)) {
                retval = 1;
            } else {
                file->arc = 0;
            }
        }
    }
    return retval;
}
