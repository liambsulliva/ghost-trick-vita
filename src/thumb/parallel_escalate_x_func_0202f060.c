struct Vec {
    int x;
    int y;
    int z;
};

void func_0202f060(struct Vec *a, const struct Vec *b)
{
    a->x = a->x - b->x;
    a->y = a->y - b->y;
    a->z = a->z - b->z;
}
