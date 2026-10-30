extern unsigned int func_0207b3dc(unsigned int value);
extern unsigned int func_0207b424(unsigned int value);
extern unsigned int func_0207c1d8(unsigned int first, unsigned int second);

struct Record60 {
    unsigned int word0;
    unsigned int word4;
    unsigned int word8;
    unsigned char byte0c;
    unsigned char byte0d;
    unsigned int word10;
    unsigned int word14;
};

int func_02006954(struct Record60 *param_1, unsigned int param_2, unsigned char param_3, unsigned int param_4, unsigned int param_5)
{
    unsigned int value;
    unsigned int other;

    param_1->word0 = param_5;
    param_1->byte0c = param_3;
    value = func_0207b3dc(param_2);
    param_1->word4 = value;
    value = func_0207b3dc(param_1->byte0c - param_2);
    other = func_0207b424(param_1->word0);
    value = func_0207c1d8(value, other);
    param_1->word8 = value;
    param_1->word14 = param_4;
    param_1->word10 = 0;
    param_1->byte0d = 7;
    return 0;
}
