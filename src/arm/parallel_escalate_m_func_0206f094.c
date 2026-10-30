typedef struct {
    int x;
    int y;
    int z;
} Vec0206f094;

void func_0206f094(const Vec0206f094 *a, const Vec0206f094 *b, Vec0206f094 *axb)
{
    int x;
    int y;
    int z;

    x = (int)(((long long)a->y * b->z - (long long)a->z * b->y + 0x800) >> 12);
    y = (int)(((long long)a->z * b->x - (long long)a->x * b->z + 0x800) >> 12);
    z = (int)(((long long)a->x * b->y - (long long)a->y * b->x + 0x800) >> 12);
    axb->x = x;
    axb->y = y;
    axb->z = z;
}
