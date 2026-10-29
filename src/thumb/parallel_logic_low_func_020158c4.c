extern int func_02031254(int size);
extern int func_02016508(int mem);
extern int func_02016a6c(int mem);

void func_020158c4(int *object)
{
    int mem;

    mem = func_02031254(0x14);
    if (mem != 0) {
        mem = func_02016508(mem);
    }
    object[0] = mem;
    mem = func_02031254(0x10);
    if (mem != 0) {
        mem = func_02016a6c(mem);
    }
    object[1] = mem;
}
