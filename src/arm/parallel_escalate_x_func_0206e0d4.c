typedef struct File0206e0d4 {
    unsigned int words[18];
} File0206e0d4;

typedef struct Table0206e0d4 {
    unsigned int base;
    unsigned int fat;
    unsigned int fat_size;
    unsigned int fnt;
    unsigned int fnt_size;
    unsigned int fat_bak;
    unsigned int fnt_bak;
    void *load_mem;
} Table0206e0d4;

typedef struct Archive0206e0d4 {
    char pad0[0x14];
    unsigned int flag;
    char pad18[8];
    Table0206e0d4 *table;
} Archive0206e0d4;

extern int func_0206e2a8(Archive0206e0d4 *arc);
extern unsigned int func_0206e214(Archive0206e0d4 *arc);
extern void func_0206c440(File0206e0d4 *file);
extern int func_0206c4f8(File0206e0d4 *file, Archive0206e0d4 *arc, unsigned int top, unsigned int bottom, unsigned int id);
extern int func_0206c704(File0206e0d4 *file, void *dst, int len);
extern int func_0206c614(File0206e0d4 *file);
extern void func_02072fa8(void *dst, int value, unsigned int size);

unsigned int func_0206e0d4(Archive0206e0d4 *arc, void *mem, unsigned int max_size)
{
    Table0206e0d4 *table;
    unsigned int total;
    unsigned char *cache;
    File0206e0d4 file;

    if (mem != 0 && func_0206e2a8(arc)) {
        func_0206e214(arc);
    }
    table = arc->table;
    total = (table->fat_size + table->fnt_size + 32 + 31) & ~31;
    if (total <= max_size) {
        cache = (unsigned char *)(((unsigned int)mem + 31) & ~31);
        func_0206c440(&file);
        if (func_0206c4f8(&file, arc, table->fat, table->fat + table->fat_size, (unsigned int)~0)) {
            if (func_0206c704(&file, cache, (int)table->fat_size) < 0) {
                func_02072fa8(cache, 0, table->fat_size);
            }
            func_0206c614(&file);
        }
        table->fat = (unsigned int)cache;
        cache += table->fat_size;
        if (func_0206c4f8(&file, arc, table->fnt, table->fnt + table->fnt_size, (unsigned int)~0)) {
            if (func_0206c704(&file, cache, (int)table->fnt_size) < 0) {
                func_02072fa8(cache, 0, table->fnt_size);
            }
            func_0206c614(&file);
        }
        table->fnt = (unsigned int)cache;
        table->load_mem = mem;
        arc->flag |= 4;
    }
    return total;
}
