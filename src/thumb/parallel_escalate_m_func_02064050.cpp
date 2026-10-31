class Filter {
public:
    virtual ~Filter();
    virtual unsigned short v08(unsigned short color, int mode);
};
extern "C" {
extern int func_02063fec(void);
extern void *func_02031254(int size);
extern Filter *func_0200eaec(void *object);

void func_02064050(void)
{
    int base;
    unsigned int x;
    unsigned int y;
    Filter *filter;
    int row;
    int off;

    base = func_02063fec();
    filter = (Filter *)func_02031254(4);
    if (filter != 0) {
        filter = func_0200eaec(filter);
    }
    x = 0;
    do {
        y = 0;
        row = base + x * 2;
        do {
            off = y * 0x200;
            *(unsigned short *)(row + off) = filter->v08(*(unsigned short *)(row + off), 0) | 0x8000;
            y = y + 1;
        } while (y < 0xc0);
        x = x + 1;
    } while (x < 0x100);
    if (filter != 0) {
        delete filter;
    }
}
}
