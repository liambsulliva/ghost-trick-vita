struct Entry {
    char pad0[0x10];
    unsigned char kind;
    char pad1[0x1f];
    unsigned short level;
};
extern char data_020ce7d4;
extern void *func_02009900(void);
extern struct Entry *func_0202fb48(void *owner, void *key, int index);
void func_02046ee0(unsigned int level)
{
    struct Entry *entry;
    entry = func_0202fb48(func_02009900(), &data_020ce7d4, -1);
    if (entry != 0 && entry->kind == 4 && entry->level < level) {
        entry->level = level;
    }
}
