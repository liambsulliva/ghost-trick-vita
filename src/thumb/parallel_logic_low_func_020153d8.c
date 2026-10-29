extern void func_02015404(int node);
extern void func_02015410(int node);

void func_020153d8(int object,int value)
{
    int self;
    int next;
    int old;

    self = object;
    next = value;
    old = *(int *)(self + 0x20);
    if (next != 0) {
        if (old != 0) {
            func_02015404(old);
        }
        *(int *)(self + 0x20) = next;
        func_02015410(next);
        return;
    }
    if (old != 0) {
        func_02015404(old);
    }
    *(int *)(self + 0x20) = 0;
}
