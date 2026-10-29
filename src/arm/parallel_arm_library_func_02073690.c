extern unsigned short data_020d0f04;
extern void func_02075f84();

void func_02073690(unsigned int unused, unsigned int flags)
{
    unsigned int bits;

    bits = flags & 0x7f00;
    bits = bits << 8;
    bits = bits >> 16;
    if (bits == 16) {
        data_020d0f04 = 1;
        return;
    }
    func_02075f84();
}
