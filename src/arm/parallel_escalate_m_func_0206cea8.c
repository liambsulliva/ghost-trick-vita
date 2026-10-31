typedef struct DirPos {
    void *arc;
    unsigned short own_id;
    unsigned short index;
    unsigned int pos;
} DirPos;

typedef struct ArchiveFNT {
    unsigned int start;
    unsigned short index;
    unsigned short parent;
} ArchiveFNT;

typedef struct SyncReadParam {
    void *arc;
    unsigned int pos;
} SyncReadParam;

typedef struct Table {
    int pad[3];
    unsigned int fnt;
} Table;

typedef struct Archive {
    int pad[8];
    Table *table;
} Archive;

typedef struct File {
    int pad0[2];
    Archive *arc;
    int pad1[5];
    DirPos pos;
    unsigned int parent;
    DirPos arg;
} File;

extern int func_0206ce1c(SyncReadParam *p, void *dst, unsigned int len);

int func_0206cea8(File *p_dir)
{
    int ret;
    Archive *const p_arc = p_dir->arc;
    Table *const table = p_arc->table;
    const DirPos *const arg = &p_dir->arg;
    ArchiveFNT fnt;
    SyncReadParam param;

    param.arc = p_arc;
    param.pos = table->fnt + arg->own_id * sizeof(fnt);
    ret = func_0206ce1c(&param, &fnt, sizeof(fnt));
    if (ret == 0) {
        p_dir->pos = *arg;
        if (arg->index == 0 && arg->pos == 0) {
            p_dir->pos.index = fnt.index;
            p_dir->pos.pos = table->fnt + fnt.start;
        }
        p_dir->parent = (unsigned int)(fnt.parent & 0xfff);
    }
    return ret;
}
