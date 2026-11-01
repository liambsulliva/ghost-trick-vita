typedef struct Disp65800 {
    char pad[0x54];
    unsigned int sub_control;
} Disp65800;

extern Disp65800 data_020c9e48;

void func_02065800(int param)
{
    unsigned char mode;
    mode = *(unsigned char *)(param + 0x14);
    if (mode == 3) {
        data_020c9e48.sub_control &= ~0x800u;
    } else if (mode == 2) {
        data_020c9e48.sub_control &= ~0x400u;
    } else if (mode == 1) {
        data_020c9e48.sub_control &= ~0x200u;
    } else if (mode == 0) {
        data_020c9e48.sub_control &= ~0x100u;
    }
}
