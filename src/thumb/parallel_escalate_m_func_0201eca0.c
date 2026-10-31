struct Entry {
    int key;
    unsigned int value : 31;
    unsigned int used : 1;
};

struct Range {
    unsigned int start;
    unsigned int count;
};

struct Table {
    char pad[0x604];
    struct Range *range;
    int pad608;
    struct Entry *entries;
};

extern int func_0201ec50(struct Table *table, unsigned int value);
extern unsigned int func_0201eb24(struct Table *table);

int func_0201eca0(struct Table *table, int key)
{
    unsigned int i;
    unsigned int count;
    struct Entry *entry;
    unsigned int value;

    i = table->range->start;
    count = table->range->count;
    for (; i < count; i++) {
        entry = &table->entries[i];
        if (entry->key == key && entry->used == 0) {
            return func_0201ec50(table, entry->value);
        }
    }
    if (key != 0) {
        table->entries[count].key = key;
        value = func_0201eb24(table);
        entry = &table->entries[table->range->count];
        entry->value = value;
        entry = &table->entries[table->range->count];
        entry->used = 1;
        table->range->count++;
    }
    return func_0201ec50(table, 0);
}
