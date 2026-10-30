struct Part {
    unsigned char before[0x12];
    unsigned char low : 3;
    unsigned char flag3 : 1;
    unsigned char flag4 : 1;
    unsigned char rest : 3;
    unsigned char value;
};

void func_02008abc(struct Part *part,unsigned char value)
{
    part->flag3 = 1;
    part->flag4 = 0;
    part->value = value;
}
