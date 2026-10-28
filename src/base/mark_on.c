struct Marked {
    unsigned char before[4];
    unsigned short kind;
    unsigned char before_18[0x12];
    unsigned char flag;
};

void func_02069c4c(int kind);

void func_02009750(struct Marked *object)
{
    func_02069c4c(object->kind);
    object->flag = 1;
}
