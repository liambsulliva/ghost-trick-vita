struct Skip02031684 {
    unsigned char skip : 1;
    unsigned char rest : 7;
};

struct Mode02031684 {
    unsigned char low : 5;
    unsigned char demo : 1;
    unsigned char rest : 2;
};

struct State02031684 {
    char pad[0xd];
    unsigned char lock;
    char pad0e[5];
    struct Skip02031684 skip_flags;
    unsigned char pad14;
    struct Mode02031684 mode_flags;
};

struct Pad02031684 {
    unsigned short held;
    unsigned short pressed;
};

extern struct State02031684 data_020cff60;
extern struct Pad02031684 data_020c9d40;
extern unsigned int func_020316dc(void);

int func_02031684(int force)
{
    if (data_020cff60.skip_flags.skip) {
        data_020cff60.skip_flags.skip = 0;
    } else if (data_020cff60.lock == 0) {
        if (data_020cff60.mode_flags.demo) {
            return 1;
        }
        if (force != 0) {
            return 1;
        }
        if ((data_020c9d40.pressed & 0x30c) != 0 && data_020c9d40.held == 0x30c && func_020316dc() == 0x30c) {
            return 1;
        }
    }
    return 0;
}
