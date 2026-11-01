typedef struct Item6dde0 {
    int f0;
    int f4;
    int f8;
    unsigned int flags;
} Item6dde0;

int func_0206dde0(void *unused, Item6dde0 *item)
{
    item->f4 = 0;
    item->flags = item->flags & ~0x30;
    return 0;
}
