typedef struct Entry {
    unsigned short a;
    unsigned short b;
    unsigned short c;
} Entry;

typedef struct State {
    int f0;
    int f4;
    int f8;
    int fc;
    int f10;
    int f14;
    int f18;
    int f1c;
} State;

extern const Entry data_020a24c8[];
extern State data_020d0e64;
extern int func_02071bec(void);

void func_02070ebc(void)
{
    int mode;

    mode = func_02071bec();
    data_020d0e64.f14 = mode;
    data_020d0e64.f4 = data_020a24c8[mode].a << 12;
    data_020d0e64.f18 = data_020a24c8[mode].b << 12;
    data_020d0e64.f1c = data_020a24c8[mode].c << 12;
}
