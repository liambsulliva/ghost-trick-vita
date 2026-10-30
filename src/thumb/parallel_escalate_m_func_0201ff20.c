struct Slot {
    char pad[5];
    unsigned char kind;
    unsigned char flags;
};

struct Table {
    char pad[0xc];
    struct Slot **slots;
};

extern unsigned char data_02084e50[];
extern unsigned int func_0201f934(struct Table *table, unsigned int index);

int func_0201ff20(struct Table *table, unsigned int index)
{
    struct Slot *slot;

    if (func_0201f934(table, index) != 0) {
        if (((unsigned int)table->slots[index]->flags << 24) >> 31) {
            return 0;
        }
        slot = table->slots[12];
        switch (index) {
        case 3:
            if (slot == 0 || ((unsigned int)slot->flags << 24) >> 31 || slot->kind != 4) {
                return 8;
            }
            return 9;
        case 4:
            if (slot == 0 || ((unsigned int)slot->flags << 24) >> 31 || slot->kind != 4) {
                return 7;
            }
            return 8;
        case 12:
            if (slot->kind == 4) {
                return 7;
            }
            return 9;
        }
        return data_02084e50[index];
    }
    return 0;
}
