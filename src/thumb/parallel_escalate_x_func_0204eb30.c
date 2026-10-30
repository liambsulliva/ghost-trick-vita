struct Range {
    short lo;
    short hi;
};

struct Span {
    int a;
    int b;
    int c;
};

extern struct Range data_0209bda0[];
extern struct Span data_0209bdf4[];

int func_0204eb30(void *object, int index, int value)
{
    int hi;
    int lo;
    int result;
    int start;
    int end;

    hi = data_0209bda0[index].hi;
    if (value >= hi) {
        return 0;
    }
    result = 0;
    lo = data_0209bda0[index].lo;
    if (value < lo) {
        start = data_0209bdf4[index].a;
        end = data_0209bdf4[index].b;
        result = start + value * (end - start) / lo;
    } else if (value < hi) {
        start = data_0209bdf4[index].b;
        end = data_0209bdf4[index].c;
        result = start + (value - lo) * (end - start) / (hi - lo);
    }
    return result >> 5;
}
