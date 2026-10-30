extern int func_020083b0();

struct Object02062b60 {
    char pad[0x28];
    int inner[0x16];
    int value;
    int extra;
};

void func_02062b60(struct Object02062b60 *param_1, int param_2, int param_3, int param_4)
{
    func_020083b0(param_1->inner, param_2);
    param_1->value = param_3;
    param_1->extra = param_4;
    if (param_3 > 0x4000) {
        param_1->value = 0x4000;
    }
    return;
}
