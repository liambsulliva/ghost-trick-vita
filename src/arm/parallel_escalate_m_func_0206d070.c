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
    int pad0[2];
    void *arc;
    int pad1[5];
    DirPos pos;
    unsigned int parent;
    union {
        DirPos seekdir;
        struct {
            DirEntry *p_entry;
            unsigned int skip_string;
        } readdir;
        struct {
            DirPos pos;
            const signed char *path;
            int find_directory;
            union {
                FileID *file;
                DirPos *dir;
            } result;
        } findpath;
    } arg;
} File;

extern int func_0206d714(File *file, unsigned int command, int block);
extern int func_0206c374(const signed char *path, int flag);
extern void func_0206ce7c(File *file, unsigned int id);

static inline int str_nicmp(const signed char *str1, const signed char *str2, unsigned int len)
{
    unsigned int i;
    int diff = 0;
    for (i = 0; i < len; ++i)
    {
        unsigned int c = (unsigned char)(str1[i] - 'A');
        unsigned int d = (unsigned char)(str2[i] - 'A');
        if (c <= 'Z' - 'A')
        {
            c += 'a' - 'A';
        }
        if (d <= 'Z' - 'A')
        {
            d += 'a' - 'A';
        }
        diff = (int)(c - d);
        if (diff != 0)
        {
            break;
        }
    }
    return diff;
}

int func_0206d070(File *p_dir)
{
    const signed char *path = p_dir->arg.findpath.path;
    const int is_dir = p_dir->arg.findpath.find_directory;

    func_0206d714(p_dir, 2, 1);
    for (; *path; path += (*path ? 1 : 0))
    {
        int name_len = func_0206c374(path, 0);
        int is_directory = (path[name_len] != '\0') || is_dir;

        if (name_len == 0)
        {
            return 6;
        }
        if (*path == '.')
        {
            if (name_len == 1)
            {
                path += 1;
                continue;
            }
            else if ((name_len == 2) && (path[1] == '.'))
            {
                if (p_dir->pos.own_id != 0)
                {
                    func_0206ce7c(p_dir, p_dir->parent);
                }
                path += 2;
                continue;
            }
        }
        else if (*path == '*')
        {
            break;
        }
        if (name_len > 127)
        {
            return 11;
        }
        {
            DirEntry etr;
            p_dir->arg.readdir.p_entry = &etr;
            p_dir->arg.readdir.skip_string = 0;
            for (;;)
            {
                if (func_0206d714(p_dir, 3, 1) != 0)
                {
                    return 11;
                }
                if ((is_directory == etr.is_directory) && (name_len == etr.name_len)
                    && (str_nicmp(path, etr.name, (unsigned int)name_len) == 0))
                {
                    break;
                }
            }
            if (is_directory)
            {
                path += name_len;
                p_dir->arg.seekdir = etr.id.dir_id;
                func_0206d714(p_dir, 2, 1);
            }
            else
            {
                if (is_dir)
                {
                    return 11;
                }
                *p_dir->arg.findpath.result.file = etr.id.file_id;
                return 0;
            }
        }
    }
    if (!is_dir)
    {
        return 11;
    }
    *p_dir->arg.findpath.result.dir = p_dir->pos;
    return 0;
}
