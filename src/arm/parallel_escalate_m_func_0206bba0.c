typedef struct FSArchive FSArchive;

typedef struct FSArchiveInterface {
    void *ReadFile;
    void *WriteFile;
    void *SeekDirectory;
    void *ReadDirectory;
    void *FindPath;
} FSArchiveInterface;

struct FSArchive {
    unsigned char reserved[0x24];
    const FSArchiveInterface *vtbl;
};

typedef struct FSDirPos {
    FSArchive *arc;
    unsigned short own_id;
    unsigned short index;
    unsigned int pos;
} FSDirPos;

typedef struct FSArchiveState {
    FSArchive *list;
    FSDirPos current;
} FSArchiveState;

typedef struct FSFile {
    struct FSFile *prev;
    struct FSFile *next;
    FSArchive *arc;
    unsigned int stat;
    void *argument;
    unsigned int reserved[13];
} FSFile;

typedef struct FSArgumentForFindPath {
    unsigned int baseid;
    const signed char *path;
    unsigned int target_id;
    int target_is_directory;
} FSArgumentForFindPath;

extern FSArchiveState data_020d0bac;
extern signed char data_020d0cbc[];
extern FSArchive *func_0206bcd4(const signed char *path, unsigned int *baseid, signed char *relpath);
extern void func_0206c440(FSFile *file);
extern int func_0206b9dc(FSFile *file, int command, int block);
extern int func_02078eb8(signed char *dst, const signed char *src, int len);

int func_0206bba0(const signed char *path)
{
    int retval = 0;
    FSArchive *arc;
    unsigned int baseid = 0;
    signed char relpath[260];

    arc = func_0206bcd4(path, &baseid, relpath);
    if (arc) {
        data_020d0bac.current.arc = arc;
        data_020d0bac.current.own_id = 0;
        data_020d0bac.current.index = 0;
        data_020d0bac.current.pos = 0;
        func_02078eb8(data_020d0cbc, relpath, 260);
        if (arc->vtbl->FindPath) {
            FSFile file;
            FSArgumentForFindPath arg;

            func_0206c440(&file);
            file.arc = arc;
            file.argument = &arg;
            arg.baseid = baseid;
            arg.path = relpath;
            arg.target_is_directory = 1;
            if (func_0206b9dc(&file, 4, 1)) {
                data_020d0bac.current.own_id = (unsigned short)arg.target_id;
                func_02078eb8(data_020d0cbc, relpath, 260);
            }
        }
        retval = 1;
    }
    return retval;
}
