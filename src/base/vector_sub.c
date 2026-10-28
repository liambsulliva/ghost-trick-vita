struct Vector {
    int x;
    int y;
    int z;
};

void func_02008398(struct Vector *vector, int x, int y, int z);

void func_020083c0(struct Vector *out, struct Vector *a, struct Vector *b)
{
    func_02008398(out, a->x - b->x, a->y - b->y, a->z - b->z);
}
