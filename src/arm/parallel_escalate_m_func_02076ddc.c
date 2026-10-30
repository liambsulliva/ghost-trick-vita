typedef struct {
    int unk0;
    int unk4;
    int unk8;
    int unkc;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
} Info02076ddc;

extern Info02076ddc data_020d1344;
extern int func_02075c98(void);
extern void func_02075cac(int);

int func_02076ddc(void)
{
    int enabled;
    int result;

    enabled = func_02075c98();
    result = data_020d1344.unk8 == 0 ? data_020d1344.unk4 : data_020d1344.unk20;
    func_02075cac(enabled);
    return result;
}
