extern unsigned char data_020c9e48[];
extern struct { char pad00[0x22]; unsigned short f22; char pad24[2]; unsigned short f26; char pad28[2]; unsigned short f2a; char pad2c[2]; unsigned short f2e; } data_020c9e88;
extern int func_02031270();

int func_020658fc(int param_1)
{
    if (*(int *)(param_1 + 4) == 0) {
        *(unsigned int *)data_020c9e48 = *(unsigned int *)(param_1 + 8);
        *(unsigned short *)(data_020c9e48 + 0xe) = *(unsigned short *)(param_1 + 0xc);
        *(unsigned short *)(data_020c9e48 + 0x12) = *(unsigned short *)(param_1 + 0xe);
        *(unsigned short *)(data_020c9e48 + 0x16) = *(unsigned short *)(param_1 + 0x10);
        *(unsigned short *)(data_020c9e48 + 0x1a) = *(unsigned short *)(param_1 + 0x12);
        *(unsigned short *)(data_020c9e48 + 6) = 0x1608;
    }
    else if (*(int *)(param_1 + 4) == 1) {
        *(unsigned int *)(data_020c9e48 + 0x54) = *(unsigned int *)(param_1 + 8);
        data_020c9e88.f22 = *(unsigned short *)(param_1 + 0xc);
        data_020c9e88.f26 = *(unsigned short *)(param_1 + 0xe);
        data_020c9e88.f2a = *(unsigned short *)(param_1 + 0x10);
        data_020c9e88.f2e = *(unsigned short *)(param_1 + 0x12);
    }
    func_02031270(param_1);
    return param_1;
}
