struct Random {
    unsigned int x;
    unsigned int y;
    unsigned int z;
    unsigned int w;
};

unsigned int func_01ff80b8(struct Random *param_1)
{
    unsigned int t;

    t = param_1->x ^ (param_1->x << 11);
    param_1->x = param_1->y;
    param_1->y = param_1->z;
    param_1->z = param_1->w;
    param_1->w = (param_1->w ^ (param_1->w >> 19)) ^ (t ^ (t >> 8));
    return param_1->w;
}
