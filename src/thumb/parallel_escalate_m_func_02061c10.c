typedef struct {
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
} Flags02061c10;

typedef struct {
    char before[0x58];
    Flags02061c10 flags;
    char middle[0xb];
    unsigned int value;
    unsigned int extra;
} View02061c10;

extern void func_020153d8(View02061c10 *object, unsigned int value);

void func_02061c10(View02061c10 *param_1,unsigned int param_2,unsigned int param_3)
{
    param_1->extra = param_3;
    param_1->value = param_2;
    param_1->flags.b0 = 1;
    param_1->flags.b1 = 1;
    param_1->flags.b3 = 1;
    func_020153d8(param_1, param_2);
}
