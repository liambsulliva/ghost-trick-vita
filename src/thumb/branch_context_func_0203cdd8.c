struct Fields {
    unsigned char before_first[200];
    unsigned short first;
    unsigned short second;
    unsigned short third;
};

void func_0203cdd8(struct Fields *object,unsigned short param_2,unsigned short param_3,unsigned short param_4)
{
    object->first = param_2;
    object->second = param_3;
    object->third = param_4;
}
