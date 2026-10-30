class Item02021dcc {
public:
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void *v10();
};

struct Table02021dcc {
    int unk0;
    int unk4;
    int unk8;
    int **entries;
    unsigned int count;
};

extern "C" {
int func_0201ee8c(void *field);

unsigned int func_02021dcc(struct Table02021dcc *table, Item02021dcc *item)
{
    int key;
    unsigned int i;

    if (item == 0) {
        return 0x7fff;
    }
    key = func_0201ee8c(item->v10());
    for (i = 0; i < table->count; i++) {
        if (key == *table->entries[i]) {
            return i;
        }
    }
    return 0x7fff;
}
}
