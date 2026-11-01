typedef struct Slot02be0 {
    char pad[0x10];
    unsigned char priority;
    unsigned char group;
    char pad2[0x3a];
} Slot02be0;

typedef struct Bank02be0 {
    char pad[0x15bc];
    Slot02be0 slots[16];
} Bank02be0;

typedef struct Game02be0 {
    char pad[0x10];
    Bank02be0 *bank;
} Game02be0;

extern Game02be0 data_020b18a0;

Slot02be0 *func_02002be0(int group)
{
    Slot02be0 *best = 0;
    int i;
    for (i = 0; i < 16; i++) {
        Slot02be0 *slot = &data_020b18a0.bank->slots[i];
        if (group == 0xff || slot->group == group) {
            if (best == 0) {
                best = slot;
            }
            if (slot->priority < best->priority) {
                best = slot;
            }
        }
    }
    return best;
}
