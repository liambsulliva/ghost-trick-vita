void func_02071188(unsigned int param_1)

{
    if ((param_1 & 1) != 0) {
        *((unsigned char *)0x04000240u) = 0x80;
    }
    if ((param_1 & 2) != 0) {
        *((unsigned char *)0x04000241u) = 0x80;
    }
    if ((param_1 & 4) != 0) {
        *((unsigned char *)0x04000242u) = 0x80;
    }
    if ((param_1 & 8) != 0) {
        *((unsigned char *)0x04000243u) = 0x80;
    }
    if ((param_1 & 0x10) != 0) {
        *((unsigned char *)0x04000244u) = 0x80;
    }
    if ((param_1 & 0x20) != 0) {
        *((unsigned char *)0x04000245u) = 0x80;
    }
    if ((param_1 & 0x40) != 0) {
        *((unsigned char *)0x04000246u) = 0x80;
    }
    if ((param_1 & 0x80) != 0) {
        *((unsigned char *)0x04000248u) = 0x80;
    }
    if ((param_1 & 0x100) != 0) {
        *((unsigned char *)0x04000249u) = 0x80;
    }
    return;
}
