struct HalfwordBlock {
    unsigned char pad[0x78];
    int first;
    int second;
    unsigned char pad2[8];
    unsigned short third;
    unsigned short fourth;
};

void func_02017e80(struct HalfwordBlock *param_1)
{
    param_1->third = 0;
    param_1->first = 0;
    param_1->second = 0;
    param_1->fourth = 0xffff;
}
