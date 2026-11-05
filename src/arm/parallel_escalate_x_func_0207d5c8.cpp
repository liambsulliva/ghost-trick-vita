typedef struct Frame7d5c8 {
    int a;
    int b;
    int c;
    int pad0c[3];
    char *base;
} Frame7d5c8;

typedef struct Range7d5c8 {
    int pad0;
    int start;
    int offset;
} Range7d5c8;

extern "C" void func_0207cbc0(Frame7d5c8 *frame, int *pc, int value);
extern "C" void func_0207e01c(Frame7d5c8 *frame, int *pc, int target);

extern "C" void func_0207d5c8(Frame7d5c8 *frame, int *pc, Range7d5c8 *range, int value)
{
    #pragma exception_terminate
    int *slot;

    func_0207cbc0(frame, pc, value);
    slot = (int *)(frame->base + range->offset);
    slot[0] = frame->b;
    slot[1] = frame->a;
    slot[2] = frame->c;
    slot[5] = value;
    func_0207e01c(frame, pc, *pc + range->start);
}
