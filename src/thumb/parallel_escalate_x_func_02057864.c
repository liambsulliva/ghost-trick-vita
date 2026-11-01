typedef struct Box57864 {
    char pad[0x28];
    int x;
    int y;
    char pad2[0x4c];
    short left;
    short bottom;
    char pad3[0xc];
    short right;
    short top;
} Box57864;

int func_02057864(Box57864 *box, int x, int y)
{
    int bottom = box->bottom << 7;
    int right = box->right << 7;
    int top = box->top << 7;
    int dx = ((x - 0x80) << 12) - box->x;
    int dy = ((0x60 - y) << 12) - box->y;
    int left = box->left << 7;
    if (dx < left - 0x8000) {
        return 0;
    }
    if (dx > right + 0x8000) {
        return 0;
    }
    if (dy < top - 0x8000) {
        return 0;
    }
    if (dy <= bottom + 0x8000) {
        return 1;
    }
    return 0;
}
