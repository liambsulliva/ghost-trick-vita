typedef struct Slot030a8 {
    int id;
    char pad[0x48];
} Slot030a8;

typedef struct Bank030a8 {
    char pad[0x15fc];
    Slot030a8 slots[16];
} Bank030a8;

typedef struct Game030a8 {
    char pad[0x10];
    Bank030a8 *bank;
} Game030a8;

extern Game030a8 data_020b18a0;

unsigned short func_020030a8(int id)
{
    unsigned short mask = 0;
    unsigned char i;
    Bank030a8 *bank = data_020b18a0.bank;
    for (i = 0; i < 16; i++) {
        int slot_id = bank->slots[i].id;
        if (slot_id == id) {
            mask |= (unsigned short)(1 << i);
        }
    }
    return mask;
}
