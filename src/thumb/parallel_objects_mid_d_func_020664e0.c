typedef struct {
    unsigned char pad[24];
    unsigned char b18;
    unsigned char b19;
    unsigned char b1a;
    unsigned int w1c;
} Frame020664e0;

void func_020664e0(Frame020664e0 *param_1, unsigned char param_2, unsigned char param_3, unsigned char param_4, unsigned int param_5)
{
    param_1->b18 = param_2;
    param_1->b19 = param_3;
    param_1->b1a = param_4;
    param_1->w1c = param_5;
    return;
}
