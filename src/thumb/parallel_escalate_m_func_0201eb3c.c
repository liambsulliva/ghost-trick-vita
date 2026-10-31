struct Entry {
    int key;
    unsigned int value : 31;
    unsigned int used : 1;
};

struct Range {
    unsigned int start;
    unsigned int count;
    unsigned int base;
};

struct Stream;
struct StreamVtbl {
    void *slots[11];
    unsigned int (*tell)(struct Stream *stream);
};
struct Stream {
    struct StreamVtbl *vtbl;
};

struct Table {
    struct Stream *stream;
    char pad[0x604 - 4];
    struct Range *range;
    int pad608;
    struct Entry *entries;
    unsigned int capacity;
};

extern void func_0201eb30(struct Table *table, unsigned int value, int flag);
extern int func_0201ec50(struct Table *table, unsigned int value);
extern unsigned int func_0201eb24(struct Table *table);

int func_0201eb3c(struct Table *table, int key)
{
    unsigned int i;
    unsigned int count;
    struct Entry *entry;
    int found;
    unsigned int pos;
    unsigned int value;

    if (key == 0) {
        return 0;
    }
    found = 0;
    i = table->range->start;
    count = table->range->count;
    for (; i < count; i++) {
        entry = &table->entries[i];
        if (entry->key == key) {
            if (entry->used) {
                pos = table->stream->vtbl->tell(table->stream);
                func_0201eb30(table, table->entries[i].value, 0);
                func_0201ec50(table, pos - table->range->base);
                func_0201eb30(table, pos, 0);
                table->entries[i].value = pos - table->range->base;
                table->entries[i].used = 0;
                return 1;
            }
            found = 1;
        }
    }
    if (found) {
        return 0;
    }
    if (count >= table->capacity) {
        return 0;
    }
    table->entries[count].key = key;
    value = func_0201eb24(table);
    entry = &table->entries[table->range->count];
    entry->value = value;
    entry = &table->entries[table->range->count];
    entry->used = 0;
    table->range->count++;
    return 1;
}
