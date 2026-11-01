typedef struct Slot02c24 {
    char pad[0x4c];
} Slot02c24;

typedef struct Bank02c24 {
    char pad[0x15bc];
    Slot02c24 slots[16];
    char pad2[0x102];
    short used;
} Bank02c24;

typedef struct Game02c24 {
    char pad[0x10];
    Bank02c24 *bank;
} Game02c24;

extern Game02c24 data_020b18a0;

Slot02c24 *func_02002c24(void)
{
    unsigned char i;
    for (i = 0; i < 16; i++) {
        if (((data_020b18a0.bank->used >> i) & 1) == 0) {
            data_020b18a0.bank->used |= (short)(1 << i);
            return &data_020b18a0.bank->slots[i];
        }
    }
    return 0;
}
