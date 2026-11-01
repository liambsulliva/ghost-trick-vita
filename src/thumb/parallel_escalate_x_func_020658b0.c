extern unsigned char data_020c9e48[];
extern unsigned char data_020c9e88[];

typedef struct Param658b0 {
    int f0;
    int mode;
    int value;
    unsigned short a;
    unsigned short b;
    unsigned short c;
    unsigned short d;
} Param658b0;

Param658b0 *func_020658b0(Param658b0 *param)
{
    if (param->mode == 0) {
        *(int *)data_020c9e48 = param->value;
        *(unsigned short *)(data_020c9e48 + 0xe) = param->a;
        *(unsigned short *)(data_020c9e48 + 0x12) = param->b;
        *(unsigned short *)(data_020c9e48 + 0x16) = param->c;
        *(unsigned short *)(data_020c9e48 + 0x1a) = param->d;
        *(unsigned short *)(data_020c9e48 + 0x6) = 0x1608;
    } else if (param->mode == 1) {
        *(int *)(data_020c9e48 + 0x54) = param->value;
        *(unsigned short *)(data_020c9e88 + 0x22) = param->a;
        *(unsigned short *)(data_020c9e88 + 0x26) = param->b;
        *(unsigned short *)(data_020c9e88 + 0x2a) = param->c;
        *(unsigned short *)(data_020c9e88 + 0x2e) = param->d;
    }
    return param;
}
