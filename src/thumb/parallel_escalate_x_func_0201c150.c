typedef struct Band1c150 {
    unsigned short hi;
    unsigned short lo;
} Band1c150;

extern Band1c150 data_020b9d14;

int func_0201c150(void *self, unsigned short value)
{
    if (value > 0x28 && value < 0x57) {
        data_020b9d14.hi = 0x57;
        data_020b9d14.lo = 0x29;
    } else if (value > 0x57 && value < 0x87) {
        data_020b9d14.hi = 0x87;
        data_020b9d14.lo = 0x58;
    } else if (value > 0x87 && value < 0xb7) {
        data_020b9d14.hi = 0xb7;
        data_020b9d14.lo = 0x88;
    } else if (value > 0xb7 && value < 0xe7) {
        data_020b9d14.hi = 0xe7;
        data_020b9d14.lo = 0xb8;
    } else if (value > 0xe7 && value < 0x117) {
        data_020b9d14.hi = 0x117;
        data_020b9d14.lo = 0xe8;
    } else {
        data_020b9d14.hi = 0x28;
        data_020b9d14.lo = 0;
    }
    return data_020b9d14.lo <= value && value < data_020b9d14.hi;
}
