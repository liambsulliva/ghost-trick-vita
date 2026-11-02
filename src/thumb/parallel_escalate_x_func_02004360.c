typedef struct Slot04360 {
    char pad[0x32];
    unsigned short flags;
    char pad2[0x18];
} Slot04360;

typedef struct Bank04360 {
    char pad[0x15bc];
    Slot04360 slots[16];
} Bank04360;

typedef struct Game04360 {
    char pad[0x10];
    Bank04360 *bank;
} Game04360;

extern Game04360 data_020b18a0;
extern void func_02007a1c(int index, unsigned short mask, int on);

void func_02004360(int index, int mask, int on)
{
    if (on != 0) {
        data_020b18a0.bank->slots[index].flags |= (unsigned short)mask;
    } else {
        data_020b18a0.bank->slots[index].flags &= (unsigned short)~mask;
    }
    func_02007a1c(index, mask, on);
}
