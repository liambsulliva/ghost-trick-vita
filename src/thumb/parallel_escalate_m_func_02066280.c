struct Table {
    char pad[0x190];
    int value[3][7];
    unsigned char count[3];
};

int func_02066280(int param_1, unsigned int param_2, unsigned int param_3)
{
    struct Table *table;

    table = *(struct Table **)(param_1 + 0xc8);
    if (table == 0) {
        return 0;
    }
    if (param_2 >= 3) {
        return -1;
    }
    if (param_3 >= 7) {
        return -1;
    }
    if (param_3 >= table->count[param_2]) {
        return -1;
    }
    return table->value[param_2][param_3];
}
