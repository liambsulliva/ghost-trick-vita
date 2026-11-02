struct Fields {
    char pad0[0x80];
    unsigned short first;
    unsigned short second;
    char pad1[0x28];
    unsigned int last;
};

void func_02052c28(struct Fields *object, unsigned int param_2, unsigned short param_3, unsigned short param_4)
{
    object->last = param_2;
    object->first = param_3;
    object->second = param_4;
}
