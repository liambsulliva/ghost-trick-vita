struct Slot {
    short id;
    short link;
    int mode;
    int value;
    short count;
    unsigned char flag_e;
    unsigned char flag_f;
    unsigned char flag_10;
    unsigned char group;
    unsigned char pad12;
    unsigned char flag_13;
    short timer;
    short pad16;
    int word_18;
    int word_1c;
    int word_20;
    unsigned char flag_24;
    char pad25[3];
    int word_28;
    int kind;
    unsigned char index;
    unsigned char pad31;
    short half_32;
    short half_34;
    short half_36;
    int word_38;
    int word_3c;
    int prev;
    int next;
    int state;
};

struct State {
    char pad0[0x13e0];
    char fill_13e0[0x20];
    char fill_1400[0x80];
    char fill_1480[0x80];
    char fill_1500[0x40];
    char fill_1540[0x40];
    int word_1580;
    char pad1584[0x24];
    int word_15a8;
    int word_15ac;
    int word_15b0;
    char pad15b4[8];
    struct Slot slots[16];
    char table_1a7c[0x100];
    unsigned char byte_1b7c;
    unsigned char byte_1b7d;
    short half_1b7e;
    short half_1b80;
    short pad1b82;
    int word_1b84;
    int word_1b88;
    int word_1b8c;
    int word_1b90;
    int word_1b94;
    int word_1b98;
    int word_1b9c;
    int word_1ba0;
    int word_1ba4;
    int pad1ba8;
    int word_1bac;
    int word_1bb0;
};

struct Root {
    char pad0[0x10];
    struct State *state;
};

extern struct Root data_020b18a0;
extern void func_020010a0(void *dest, int size);
extern void func_02000f70(void *dest, int value, int size);

void func_02003900(void)
{
    int i;
    struct Slot *slot;

    func_020010a0(data_020b18a0.state->slots, sizeof(data_020b18a0.state->slots));
    func_020010a0(data_020b18a0.state->table_1a7c, sizeof(data_020b18a0.state->table_1a7c));
    func_02000f70(data_020b18a0.state->fill_13e0, -1, sizeof(data_020b18a0.state->fill_13e0));
    func_02000f70(data_020b18a0.state->fill_1400, 0, sizeof(data_020b18a0.state->fill_1400));
    func_02000f70(data_020b18a0.state->fill_1480, 0, sizeof(data_020b18a0.state->fill_1480));
    func_02000f70(data_020b18a0.state->fill_1500, 0, sizeof(data_020b18a0.state->fill_1500));
    func_02000f70(data_020b18a0.state->fill_1540, -1, sizeof(data_020b18a0.state->fill_1540));
    data_020b18a0.state->byte_1b7d = 0;
    data_020b18a0.state->word_1b98 = -1;
    data_020b18a0.state->word_1b9c = 0;
    data_020b18a0.state->word_1ba0 = 0;
    data_020b18a0.state->word_1ba4 = 0;
    data_020b18a0.state->word_1bac = 0;
    data_020b18a0.state->word_1bb0 = 0;
    data_020b18a0.state->half_1b7e = 0;
    data_020b18a0.state->byte_1b7c = 0;
    data_020b18a0.state->half_1b80 = 0;
    data_020b18a0.state->word_1580 = 0;
    for (i = 0; i < 16; i++) {
        slot = &data_020b18a0.state->slots[i];
        slot->index = i;
        slot->half_32 = 0;
        slot->half_34 = 0;
        slot->half_36 = 0;
        slot->word_38 = 0;
        slot->word_3c = 0;
        slot->state = 2;
        slot->id = -1;
        slot->link = -1;
        slot->group = 0xff;
        slot->mode = 3;
        slot->count = 0;
        slot->flag_e = 0;
        slot->flag_f = 0;
        slot->flag_10 = 0;
        slot->value = 0;
        slot->flag_13 = 0;
        slot->timer = 0;
        slot->word_18 = 0;
        slot->word_1c = 0;
        slot->word_20 = 0;
        slot->flag_24 = 0;
        slot->word_28 = 0;
        slot->kind = 3;
        slot->prev = -1;
        slot->next = -1;
    }
    data_020b18a0.state->word_15a8 = 0;
    data_020b18a0.state->word_15ac = 0;
    data_020b18a0.state->word_15b0 = 0;
    data_020b18a0.state->word_1b84 = 0;
    data_020b18a0.state->word_1b88 = 0;
    data_020b18a0.state->word_1b8c = 0;
    data_020b18a0.state->word_1b90 = 0;
    data_020b18a0.state->word_1b94 = 0;
}
