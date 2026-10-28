struct Vector {
    int x;
    int y;
    int z;
};

void func_02008398(struct Vector *vector, int x, int y, int z);
void func_02009bfc(void *object, struct Vector *vector);

void func_02009bdc(void *object, int x, int y)
{
    struct Vector vector;
    func_02008398(&vector, (short)x << 12, (short)y << 12, 0);
    func_02009bfc(object, &vector);
}
