typedef struct DirPos {
    void *arc;
    unsigned short own_id;
    unsigned short index;
    unsigned int pos;
} DirPos;

typedef struct FileID {
    void *arc;
    unsigned int file_id;
} FileID;

typedef struct DirEntry {
    union {
        FileID file_id;
        DirPos dir_id;
    } id;
    unsigned int is_directory;
    unsigned int name_len;
    signed char name[128];
} DirEntry;

typedef struct File {
    void *link[2];
    void *arc;
    unsigned int stat;
    int pad[4];
    union {
        struct {
            unsigned int own_id;
            unsigned int top;
            unsigned int bottom;
            unsigned int pos;
        } file;
        struct {
            DirPos pos;
            unsigned int parent;
        } dir;
    } prop;
    union {
        struct {
            DirEntry *p_entry;
            unsigned int skip_string;
        } readdir;
        struct {
            signed char *buf;
            unsigned int buf_len;
            unsigned short total_len;
            unsigned short dir_id;
        } getpath;
        unsigned char raw[0x18];
    } arg;
} File;

extern signed char data_020b1734[];
extern void func_0206c440(File *file);
extern void func_0206ce7c(File *file, unsigned int id);
extern int func_0206d714(File *file, unsigned int command, int block);
extern const signed char *func_0206c0e0(void *arc);
extern int func_02079020(const signed char *s);
extern void func_0207303c(const void *src, void *dst, unsigned int size);

static inline int is_dir(volatile const File *file)
{
    return (file->stat & 0x20) ? 1 : 0;
}

int func_0206d284(File *p_file)
{
    DirEntry entry;
    File tmp;
    void *p_arc;
    unsigned int dir_id;
    unsigned int file_id;
    unsigned int id;
    int total;

    p_arc = p_file->arc;
    func_0206c440(&tmp);
    tmp.arc = p_arc;
    if (is_dir(p_file)) {
        dir_id = p_file->prop.dir.pos.own_id;
        file_id = 0x10000;
    } else {
        unsigned int pos = 0;
        unsigned int num_dir = 0;
        file_id = p_file->prop.file.own_id;
        dir_id = 0x10000;
        do {
            func_0206ce7c(&tmp, pos);
            if (pos == 0) {
                num_dir = tmp.prop.dir.parent;
            }
            tmp.arg.readdir.p_entry = &entry;
            tmp.arg.readdir.skip_string = 1;
            while (func_0206d714(&tmp, 3, 1) == 0) {
                if (entry.is_directory == 0 && entry.id.file_id.file_id == file_id) {
                    dir_id = tmp.prop.dir.pos.own_id;
                    break;
                }
            }
        } while (dir_id == 0x10000 && ++pos < num_dir);
    }
    if (dir_id == 0x10000) {
        p_file->arg.getpath.total_len = 0;
        return 11;
    }
    id = dir_id;
    total = func_02079020(func_0206c0e0(p_arc)) + 2;
    func_0206ce7c(&tmp, dir_id);
    if (file_id != 0x10000) {
        total += entry.name_len;
    }
    while (id != 0) {
        func_0206ce7c(&tmp, tmp.prop.dir.parent);
        tmp.arg.readdir.p_entry = &entry;
        tmp.arg.readdir.skip_string = 1;
        while (func_0206d714(&tmp, 3, 1) == 0) {
            if (entry.is_directory != 0 && entry.id.dir_id.own_id == id) {
                total += entry.name_len + 1;
                break;
            }
        }
        id = tmp.prop.dir.pos.own_id;
    }
    {
        unsigned int pos;
        signed char *buf = p_file->arg.getpath.buf;
        p_file->arg.getpath.total_len = (unsigned short)(total + 1);
        p_file->arg.getpath.dir_id = (unsigned short)dir_id;
        if (buf != 0) {
            pos = p_file->arg.getpath.total_len;
            if (p_file->arg.getpath.buf_len >= pos) {
                int len;
                const signed char *arc_name;
                int off = 0;
                arc_name = func_0206c0e0(p_arc);
                len = func_02079020(arc_name);
                func_0207303c(arc_name, buf + off, (unsigned int)len);
                off += len;
                func_0207303c(data_020b1734, buf + off, 2);
                func_0206ce7c(&tmp, dir_id);
                if (file_id != 0x10000) {
                    tmp.arg.readdir.p_entry = &entry;
                    tmp.arg.readdir.skip_string = 0;
                    while (func_0206d714(&tmp, 3, 1) == 0) {
                        if (entry.is_directory == 0 && entry.id.file_id.file_id == file_id) {
                            break;
                        }
                    }
                    {
                        unsigned int n = entry.name_len + 1;
                        func_0207303c(entry.name, buf + pos - n, n);
                        pos -= n;
                    }
                } else {
                    *(buf + pos - 1) = 0;
                    pos -= 1;
                }
                id = dir_id;
                while (id != 0) {
                    func_0206ce7c(&tmp, tmp.prop.dir.parent);
                    tmp.arg.readdir.p_entry = &entry;
                    tmp.arg.readdir.skip_string = 0;
                    buf[pos - 1] = '/';
                    pos -= 1;
                    while (func_0206d714(&tmp, 3, 1) == 0) {
                        if (entry.is_directory != 0 && entry.id.dir_id.own_id == id) {
                            unsigned int n = entry.name_len;
                            func_0207303c(entry.name, buf + pos - n, n);
                            pos -= n;
                            break;
                        }
                    }
                    id = tmp.prop.dir.pos.own_id;
                }
            }
        }
    }
    return 0;
}
