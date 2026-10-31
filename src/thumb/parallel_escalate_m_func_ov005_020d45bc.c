extern unsigned char data_020c9e48[];
extern struct { char pad00[0x12]; short f12; char pad14[8]; short f1c; short f1e; } data_020c9e88;
extern struct { char pad00[0x26]; short f26; } data_020c9ec8;
extern int func_ov005_020d40a8();
extern int func_02071d24();
extern int func_02071d38();
extern int func_02071d94();
extern int func_02071dd0();
extern int func_02071de4();
extern int func_02071df8();
extern int func_020716e8();
extern int func_02071220();
extern int func_020719fc();
extern int func_02071aec();

void func_ov005_020d45bc(void)
{
    int value;
    unsigned int field;
    unsigned int part;

    func_ov005_020d40a8(1);
    func_02071d94();
    func_02071d38();
    func_02071d24();
    func_02071dd0();
    func_02071de4();
    func_02071df8();
    func_020716e8(5);
    func_02071220(2);
    value = 8;
    func_020719fc(value);
    func_02071aec(0x100);
    field = *(unsigned int *)(data_020c9e48 + 0x54);
    value = value - 0x18;
    part = 7;
    part = part << 10;
    field = (field & 0xfffffcffu) | part;
    part = part << 20;
    field = part | field;
    *(unsigned int *)(data_020c9e48 + 0x54) = field;
    data_020c9e88.f12 = value;
    data_020c9ec8.f26 = value;
    data_020c9e88.f1c = 0x44c6;
    data_020c9e88.f1e = 0x6d7;
}
