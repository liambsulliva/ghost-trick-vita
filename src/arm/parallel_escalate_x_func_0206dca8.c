typedef struct Range6dca8 {
    int f0;
    int top;
    int bottom;
    int pos;
} Range6dca8;

typedef struct File6dca8 {
    int f0;
    Range6dca8 *range;
} File6dca8;

int func_0206dca8(void *archive, File6dca8 *file, int *offset, int whence)
{
    Range6dca8 *range = file->range;
    int pos = *offset;
    switch (whence) {
    case 0:
        pos += range->top;
        break;
    case 1:
    default:
        pos += range->pos;
        break;
    case 2:
        pos += range->bottom;
        break;
    }
    if (pos < range->top || pos > range->bottom) {
        return 6;
    }
    range->pos = pos;
    *offset = pos;
    return 0;
}
