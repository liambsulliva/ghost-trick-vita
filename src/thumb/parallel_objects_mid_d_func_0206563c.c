extern unsigned int data_020c9e48;
void func_0206563c(int param_1)
{
    unsigned char mode;
    mode = *(unsigned char *)(param_1 + 0x14);
    if (mode == 3) {
        data_020c9e48 |= 0x800;
    } else if (mode == 2) {
        data_020c9e48 |= 0x400;
    } else if (mode == 1) {
        data_020c9e48 |= 0x200;
    } else if (mode == 0) {
        data_020c9e48 |= 0x100;
    }
}
