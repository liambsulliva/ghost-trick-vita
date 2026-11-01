typedef struct Disp14798 {
    unsigned int control;
    unsigned short h4;
    unsigned short h6;
    unsigned short h8;
    unsigned short ha;
    char pad[0x48];
    unsigned int sub_control;
} Disp14798;

typedef struct Sub14798 {
    char pad[0x18];
    unsigned short h18;
    unsigned short h1a;
    unsigned short h1c;
    unsigned short h1e;
} Sub14798;

extern Disp14798 data_020c9e48;
extern Sub14798 data_020c9e88;

void func_02014798(void)
{
    data_020c9e48.sub_control = 0xc0210010;
    data_020c9e48.control = 0x00211918;
    data_020c9e48.h4 = 0x1609;
    data_020c9e48.h6 = 0x1608;
    data_020c9e48.h8 = 0x370f;
    data_020c9e48.ha = 0x92;
    data_020c9e88.h18 = 0;
    data_020c9e88.h1a = 0x205;
    data_020c9e88.h1c = 0x409;
    data_020c9e88.h1e = 0xfc7;
}
