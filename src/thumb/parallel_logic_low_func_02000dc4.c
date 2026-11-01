typedef struct Entry02dc4 {
    unsigned int id;
    int size;
} Entry02dc4;

typedef struct Bank02dc4 {
    char pad[0x1b84];
    Entry02dc4 *table;
    unsigned int pad2;
    unsigned int count;
} Bank02dc4;

typedef struct Game02dc4 {
    char pad[0x10];
    Bank02dc4 *bank;
} Game02dc4;

extern Game02dc4 data_020b18a0;

int func_02000dc4(unsigned int id)
{
    unsigned int i;
    unsigned int count;

    count = data_020b18a0.bank->count;
    for (i = 0; i < count; i++) {
        if (data_020b18a0.bank->table[i].id == id) {
            return data_020b18a0.bank->table[i].size;
        }
    }
    return -1;
}
