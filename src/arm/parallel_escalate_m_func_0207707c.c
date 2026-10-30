typedef struct {
    unsigned int first;
    unsigned int second;
    unsigned char count;
} Entry;

extern Entry data_020d2e20[];

unsigned char func_0207707c(int index,unsigned int first,unsigned int second)
{
    Entry *entry;

    data_020d2e20[index].first = first;
    entry = &data_020d2e20[index];
    entry->second = second;
    return ++entry->count;
}
