struct PackedWord {
    unsigned int lower : 16;
    unsigned int upper : 16;
};

void func_02026700(int object,int value)
{
    ((struct PackedWord *)(object + 20))->upper = value;
}
