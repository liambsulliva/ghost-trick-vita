struct Entry {
    unsigned char pad[6];
    unsigned char flags;
    unsigned char order;
};
struct Table {
    int pad0;
    int next;
    unsigned int count;
    struct Entry **entries;
};
extern unsigned int func_0201f934(struct Table *table, unsigned int index);
extern void *func_02008444(void);
extern int func_0201fa54(void *object);
extern struct Entry *func_0201f698(int size);
extern struct Entry *func_0201f598(struct Entry *entry);
extern void func_0201f7e4(void *object);
extern struct Entry *func_0201f6fc(struct Entry *entry, int arg, unsigned char layer);

void func_0201f974(struct Table *table, unsigned int index, unsigned char arg, signed char layer)
{
    int selected_layer;
    int count;
    int changed;
    int off;
    struct Entry *entry;
    struct Entry *other;
    int best;
    unsigned int i;
    int order;
    int next;

    if (func_0201f934(table, index) != 0 && index != 0) {
        selected_layer = layer;
        if (selected_layer == -1) {
            selected_layer = (signed char)func_0201fa54(func_02008444());
        }
        off = index * 4;
        entry = *(struct Entry **)((char *)table->entries + off);
        changed = 0;
        order = 0;
        if (entry == 0) {
            entry = func_0201f698(0xc);
            if (entry != 0) {
                entry = func_0201f598(entry);
            }
            if (entry != 0) {
                next = table->next;
                table->next = next + 1;
                entry->order = next;
                entry->flags |= 2;
                func_0201f7e4(func_02008444());
            }
        } else if (((unsigned int)entry->flags << 24) >> 31) {
            best = -1;
            count = table->count;
            for (i = 0; i < count; i++) {
                other = table->entries[i];
                if (other != 0 && (((unsigned int)other->flags << 24) >> 31) == 0 && best < other->order) {
                    best = other->order;
                }
            }
            if (entry->order != best + 1) {
                changed = 1;
                order = (unsigned char)(best + 1);
            }
        }
        if (entry != 0) {
            *(struct Entry **)((char *)table->entries + off) = func_0201f6fc(entry, arg, selected_layer);
            if (changed != 0 && *(struct Entry **)((char *)table->entries + off) != 0) {
                (*(struct Entry **)((char *)table->entries + off))->order = order;
            }
        }
    }
}
