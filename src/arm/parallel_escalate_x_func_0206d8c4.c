typedef struct DirId8c4 {
    unsigned short own;
    unsigned short index;
} DirId8c4;

typedef union Id8c4 {
    unsigned int file;
    DirId8c4 dir;
} Id8c4;

typedef struct Found8c4 {
    int pad0;
    Id8c4 id;
    int pad8;
    int is_dir;
    unsigned int len;
    char name[0x80];
} Found8c4;

typedef struct Fat8c4 {
    unsigned int top;
    unsigned int bottom;
} Fat8c4;

typedef struct Table8c4 {
    int pad0;
    unsigned int fat;
    unsigned int fat_size;
} Table8c4;

typedef struct Archive8c4 {
    int pad[8];
    Table8c4 *table;
} Archive8c4;

typedef struct File8c4 {
    char pad[0x30];
    Found8c4 *arg;
    int arg2;
} File8c4;

typedef struct Info8c4 {
    char pad0[0x10];
    int mode;
    char name[0x104];
    unsigned int len;
    unsigned int attr;
    char pad120[0x18];
    int zero[6];
    char pad150[0x18];
    unsigned int size;
    unsigned int id;
} Info8c4;

typedef struct SyncReadParam8c4 {
    Archive8c4 *arc;
    unsigned int pos;
} SyncReadParam8c4;

extern int func_0206d714(File8c4 *file, unsigned int command, int block);
extern void func_0207303c(const void *src, void *dst, unsigned int size);
extern int func_0206ce1c(SyncReadParam8c4 *p, void *dst, unsigned int len);
extern int func_0206e2c4(Archive8c4 *arc, unsigned int top);

int func_0206d8c4(Archive8c4 *arc, File8c4 *file, Info8c4 *info)
{
    Found8c4 found;
    Fat8c4 fat;
    SyncReadParam8c4 param;
    int ret;
    unsigned int off;

    file->arg = &found;
    file->arg2 = 0;
    ret = func_0206d714(file, 3, 1);
    if (ret == 0) {
        info->mode = 0;
        info->len = found.len;
        func_0207303c(found.name, info->name, info->len);
        info->name[info->len] = 0;
        if (found.is_dir) {
            info->attr = 0x100;
            info->id = found.id.dir.own | (found.id.dir.index << 16);
            info->size = 0;
        } else {
            info->attr = 0;
            info->id = found.id.file;
            info->size = 0;
            off = info->id * 8;
            if (off < arc->table->fat_size) {
                param.arc = arc;
                param.pos = arc->table->fat + off;
                if (func_0206ce1c(&param, &fat, 8) == 0) {
                    info->size = fat.bottom - fat.top;
                    if (func_0206e2c4(arc, fat.top) != 0) {
                        info->attr |= 0x400;
                    }
                }
            }
        }
        info->zero[0] = 0;
        info->zero[1] = 0;
        info->zero[2] = 0;
        info->zero[3] = 0;
        info->zero[4] = 0;
        info->zero[5] = 0;
    }
    return ret;
}
