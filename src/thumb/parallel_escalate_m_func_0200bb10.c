struct Node {
    unsigned char before[0xc];
    unsigned char bit0 : 1;
    unsigned char bit1 : 1;
    unsigned char rest : 6;
    unsigned char after[0xb];
    unsigned int value;
};

void func_0200bb10(struct Node *node,unsigned int value)
{
    node->bit1 = 1;
    node->value = value;
}
