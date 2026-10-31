extern unsigned char data_020c9e48[];
extern struct { char pad00[0x18]; unsigned short f18; char pad1a[2]; unsigned short f1c; } data_020c9e88;
extern int func_02071960();
extern int func_02071dd0();

void func_020659a0(int param_1)
{
    if (*(int *)(param_1 + 4) == 0) {
        *(unsigned int *)data_020c9e48 = 0x210a10;
        *(unsigned short *)(data_020c9e48 + 6) = 0x8209;
        *(unsigned short *)(data_020c9e48 + 10) = 0x8011;
        return;
    }
    if (*(int *)(param_1 + 4) == 1) {
        func_02071dd0();
        func_02071960(4);
        *(unsigned int *)(data_020c9e48 + 0x54) = 0x210510;
        data_020c9e88.f18 = 0x8005;
        data_020c9e88.f1c = 0x8215;
    }
}
