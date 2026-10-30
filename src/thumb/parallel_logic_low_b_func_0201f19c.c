extern void *func_0201f1d0(int size);
extern void *func_0201f1e0(void *object);
extern void func_0201f1d8(int object,int first,int second);
extern void func_0200c530(int object,int data);

void func_0201f19c(int *object,int first,int second)
{
    void *mem;

    if (object[0] == 0) {
        mem = func_0201f1d0(0xc);
        if (mem != 0) {
            mem = func_0201f1e0(mem);
        }
        object[0] = (int)mem;
        func_0201f1d8((int)mem,first,second);
        func_0200c530(object[0],0x02084e14);
    }
}
