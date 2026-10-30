extern unsigned int data_020adcc4;
extern void func_020569f0();
extern void func_0202e46c();
extern void func_0202b164();
extern void func_02056b18();

unsigned int *func_02056b2c(unsigned int *param_1)
{
    unsigned int *slot;
    unsigned char *item;
    unsigned char *end;

    func_020569f0(param_1);
    *param_1 = (unsigned int)&data_020adcc4;
    slot = param_1 + 0x1f;
    do {
        func_0202e46c(slot);
        slot = slot + 2;
    } while (slot < param_1 + 0x27);
    item = (unsigned char *)(param_1 + 0x29);
    end = (unsigned char *)(param_1 + 0x31);
    do {
        func_0202b164(item);
        item = item + 8;
    } while (item < end);
    *end = *end & ~0xf;
    func_02056b18(param_1,0x7fff);
    return param_1;
}
