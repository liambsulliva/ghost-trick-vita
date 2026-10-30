typedef struct {
    int x;
    int y;
    int z;
} VecFx32;

int func_0206f058(const VecFx32 *a, const VecFx32 *b)
{
    return (int)(((long long)a->x * b->x + (long long)a->y * b->y + (long long)a->z * b->z + (1 << 11)) >> 12);
}
