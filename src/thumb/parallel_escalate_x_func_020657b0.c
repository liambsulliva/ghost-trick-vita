typedef struct Disp657b0 {
    char pad[0x54];
    unsigned int sub_control;
} Disp657b0;

extern Disp657b0 data_020c9e48;

void func_020657b0(int param)
{
    unsigned char mode;
    mode = *(unsigned char *)(param + 0x14);
    if (mode == 3) {
        data_020c9e48.sub_control |= 0x800;
    } else if (mode == 2) {
        data_020c9e48.sub_control |= 0x400;
    } else if (mode == 1) {
        data_020c9e48.sub_control |= 0x200;
    } else if (mode == 0) {
        data_020c9e48.sub_control |= 0x100;
    }
}
