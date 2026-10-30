struct Slot02073480 {
    unsigned int value;
    int active;
    int count;
};

extern struct Slot02073480 data_020d0ea4[];
extern unsigned int data_027e0000[];

void func_02073480(unsigned int mask, unsigned int value)
{
    int i;
    struct Slot02073480 *slot;

    for (i = 0; i < 22; i++) {
        if (mask & 1) {
            slot = 0;
            if (i >= 8 && i <= 11) {
                slot = &data_020d0ea4[i - 8];
            } else if (i >= 3 && i <= 6) {
                slot = &data_020d0ea4[i + 1];
            } else {
                data_027e0000[i] = value;
            }
            if (slot != 0) {
                slot->value = value;
                slot->count = 0;
                slot->active = 1;
            }
        }
        mask >>= 1;
    }
}
