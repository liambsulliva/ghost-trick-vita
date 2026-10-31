typedef struct {
    void *arc;
    unsigned short own_id;
    unsigned short index;
    unsigned int pos;
} DirPos;

typedef struct {
    void *arc;
    unsigned int file_id;
} FileID;

typedef struct {
    union {
        FileID file_id;
        DirPos dir_id;
    } u;
    unsigned int is_directory;
    unsigned int name_len;
    char name[128];
} DirEntry;

typedef struct {
    char pad0[8];
    void *arc;
    char pad1[0x14];
    DirPos pos;
    char pad2[4];
    DirEntry *p_entry;
    int skip_string;
} File;

typedef struct {
    void *arc;
    unsigned int pos;
} SyncReadParam;

extern int func_0206ce1c(SyncReadParam *p, void *dst, unsigned int len);

int func_0206cf44(File *p_dir)
{
    DirEntry *p_entry = p_dir->p_entry;
    int ret;
    SyncReadParam param;
    param.arc = p_dir->arc;
    param.pos = p_dir->pos.pos;
    {
        unsigned char len;
        ret = func_0206ce1c(&param, &len, sizeof(len));
        if (ret != 0)
            return ret;
        p_entry->name_len = (unsigned int)(len & 0x7f);
        p_entry->is_directory = (unsigned int)((len >> 7) & 1);
    }
    if (p_entry->name_len == 0)
        return 1;
    if (!p_dir->skip_string) {
        ret = func_0206ce1c(&param, p_entry->name, p_entry->name_len);
        if (ret != 0)
            return ret;
        p_entry->name[p_entry->name_len] = '\0';
    } else {
        param.pos += p_entry->name_len;
    }
    if (!p_entry->is_directory) {
        p_entry->u.file_id.arc = p_dir->arc;
        p_entry->u.file_id.file_id = p_dir->pos.index;
        ++p_dir->pos.index;
    } else {
        unsigned short id;
        ret = func_0206ce1c(&param, &id, sizeof(id));
        if (ret == 0) {
            p_entry->u.dir_id.arc = p_dir->arc;
            p_entry->u.dir_id.own_id = (unsigned short)(id & 0x0fff);
            p_entry->u.dir_id.index = 0;
            p_entry->u.dir_id.pos = 0;
        }
    }
    if (ret == 0)
        p_dir->pos.pos = param.pos;
    return ret;
}
