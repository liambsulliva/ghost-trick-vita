typedef struct ArchiveFAT {
    unsigned int top;
    unsigned int bottom;
} ArchiveFAT;

typedef struct SyncReadParam {
    void *arc;
    unsigned int pos;
} SyncReadParam;

typedef struct Table {
    int pad;
    unsigned int fat;
    unsigned int fat_size;
} Table;

typedef struct Archive {
    int pad[8];
    Table *table;
} Archive;

typedef struct File {
    int pad0[2];
    Archive *arc;
    int pad1[9];
    unsigned int top;
    unsigned int bottom;
    unsigned int index;
} File;

extern int func_0206ce1c(SyncReadParam *p, void *dst, unsigned int len);
extern int func_0206d714(File *file, unsigned int command, int block);

int func_0206d600(File *p_file)
{
    int ret;
    Archive *const p_arc = p_file->arc;
    Table *const table = p_arc->table;
    const unsigned int index = p_file->bottom;
    unsigned int pos = index * sizeof(ArchiveFAT);

    if (pos >= table->fat_size)
        return 11;
    {
        ArchiveFAT fat;
        SyncReadParam param;
        param.arc = p_arc;
        param.pos = table->fat + pos;
        ret = func_0206ce1c(&param, &fat, sizeof(fat));
        if (ret != 0)
            return ret;
        p_file->top = fat.top;
        p_file->bottom = fat.bottom;
        p_file->index = index;
        return func_0206d714(p_file, 7, 1);
    }
}
