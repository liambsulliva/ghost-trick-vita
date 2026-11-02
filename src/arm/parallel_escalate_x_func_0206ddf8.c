typedef struct {
    void *arc;
    unsigned int pos;
} SyncReadParam;

typedef struct {
    unsigned int top;
    unsigned int bottom;
} FatEntry;

typedef struct {
    unsigned int attributes;
    unsigned int times[18];
    unsigned int filesize;
    unsigned int id;
} PathInfo;

typedef struct {
    unsigned int pad0;
    unsigned int fat;
    unsigned int fat_size;
} FatTable;

typedef struct {
    char pad0[0x20];
    FatTable *table;
} RomArchive;

extern void func_02072fa8(void *dst, int value, unsigned int size);
extern int func_0206d9ec(void *arc, unsigned int base_id, const char *path, unsigned int *out, int is_dir);
extern int func_0206ce1c(SyncReadParam *p, void *dst, unsigned int len);
extern unsigned int func_0206e2c4(RomArchive *arc, unsigned int top);

int func_0206ddf8(RomArchive *arc, unsigned int base_id, const char *path, PathInfo *info)
{
    int result = 5;
    unsigned int id = 0;

    func_02072fa8(info, 0, sizeof(PathInfo));
    if (func_0206d9ec(arc, base_id, path, &id, 1) == 0) {
        info->attributes = 0x100;
        info->id = id;
        result = 0;
    } else if (func_0206d9ec(arc, base_id, path, &id, 0) == 0) {
        info->attributes = 0;
        info->id = id;
        info->filesize = 0;
        {
            FatTable *context = arc->table;
            unsigned int pos = id * sizeof(FatEntry);
            if (pos < context->fat_size) {
                FatEntry fat;
                SyncReadParam tbl;
                tbl.arc = arc;
                tbl.pos = context->fat + pos;
                if (func_0206ce1c(&tbl, &fat, sizeof(fat)) == 0) {
                    info->filesize = fat.bottom - fat.top;
                    if (func_0206e2c4(arc, fat.top) != 0) {
                        info->attributes |= 0x400;
                    }
                }
            }
        }
        result = 0;
    }
    info->attributes |= 0x200;
    return result;
}
