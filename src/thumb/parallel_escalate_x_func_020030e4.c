typedef struct Info030e4 {
    char pad[4];
    unsigned char kind;
    unsigned char group;
} Info030e4;

typedef struct Slot030e4 {
    char pad[0x40];
    int id;
    Info030e4 *info;
    char pad2[4];
} Slot030e4;

typedef struct Bank030e4 {
    char pad[0x15bc];
    Slot030e4 slots[16];
} Bank030e4;

typedef struct Game030e4 {
    char pad[0x10];
    Bank030e4 *bank;
} Game030e4;

extern Game030e4 data_020b18a0;

unsigned short func_020030e4(int kind, int group)
{
    unsigned short mask = 0;
    unsigned char i;
    for (i = 0; i < 16; i++) {
        Slot030e4 *slot = &data_020b18a0.bank->slots[i];
        if (slot->id != -1) {
            Info030e4 *info = slot->info;
            if ((info->kind == kind || kind == 0xff) && (info->group == group || group == 0xff)) {
                mask |= (unsigned short)(1 << i);
            }
        }
    }
    return mask;
}
