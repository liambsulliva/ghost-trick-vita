struct Table {
    unsigned char pad[5];
    unsigned char locked;
    unsigned short keys[4];
    unsigned short values[4];
};

int func_02012218(struct Table *table, unsigned int key, int value)
{
    unsigned int i;
    unsigned int j;
    if (table->locked != 0) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        if (key == table->keys[i]) {
            table->values[i] = value;
            return 1;
        }
        if (table->keys[i] > key) {
            for (j = 3; j > i; j--) {
                table->keys[j] = table->keys[j - 1];
                table->values[j] = table->values[j - 1];
            }
            table->keys[i] = key;
            table->values[i] = value;
            return 1;
        }
    }
    return 0;
}
