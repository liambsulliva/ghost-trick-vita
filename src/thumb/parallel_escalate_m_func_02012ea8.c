struct Pair {
    int x;
    int y;
};
struct Frame {
    unsigned int first;
    unsigned short kind;
    unsigned short tag;
    unsigned int second;
    unsigned int third;
    unsigned int unused10;
    unsigned int fourth;
    unsigned int unused18;
    struct Pair a;
    struct Pair b;
    int zero0;
    int zero1;
    int zero2;
};
extern struct Pair data_020a2938;

void func_02012ea8(struct Frame *frame, unsigned int second, unsigned int first, unsigned int third, unsigned short tag, unsigned int fourth)
{
    frame->first = first;
    frame->kind = 4;
    frame->tag = tag;
    frame->second = second;
    frame->third = third;
    frame->fourth = fourth;
    frame->a = data_020a2938;
    frame->b = data_020a2938;
    frame->zero0 = 0;
    frame->zero1 = 0;
    frame->zero2 = 0;
}
