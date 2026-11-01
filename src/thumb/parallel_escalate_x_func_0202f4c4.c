typedef struct Entry2f4c4 {
    int words[4];
} Entry2f4c4;

typedef struct List2f4c4 {
    unsigned int count;
    Entry2f4c4 entries[1];
} List2f4c4;

extern void func_0202f3b0(List2f4c4 *list, Entry2f4c4 *entry);

void func_0202f4c4(List2f4c4 *list)
{
    Entry2f4c4 *entry = list->entries;
    unsigned int i = 0;
    if (list->count > i) {
        while (i < list->count) {
            func_0202f3b0(list, entry);
            i++;
            entry++;
        }
    }
}
