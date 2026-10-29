struct Fields {
    unsigned char before_first[4];
    unsigned char first;
    unsigned char between[86];
    unsigned char second;
    unsigned char third;
};

void func_020373f8(struct Fields *object,unsigned char param_2,unsigned char param_3,unsigned char param_4)
{
    object->first = param_2;
    object->second = param_3;
    object->third = param_4;
}
