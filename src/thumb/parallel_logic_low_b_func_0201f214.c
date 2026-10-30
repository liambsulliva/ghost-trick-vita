extern void *func_0201f24c(int size);
extern void *func_0201f254(void *object);
extern void func_0201e144(int object,int first,int second,unsigned int size);
extern void func_0200c530(int object,int data);

void func_0201f214(int *object,int first,int second)
{
    void *mem;

    if (object[1] == 0) {
        mem = func_0201f24c(0x3c);
        if (mem != 0) {
            mem = func_0201f254(mem);
        }
        object[1] = (int)mem;
        func_0201e144((int)mem,first,second,4);
        func_0200c530(object[1],0x02084e30);
    }
}
