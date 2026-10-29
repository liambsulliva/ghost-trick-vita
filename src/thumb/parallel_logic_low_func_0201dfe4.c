extern int func_02031254(int size);
extern int func_0201df0c(int mem,unsigned int index,int count);

int func_0201dfe4(int *object,unsigned int index)
{
    int mem;

    if (index > 3) {
        index = 3;
    }
    if (*object == 0) {
        mem = func_02031254(0x8c);
        if (mem != 0) {
            mem = func_0201df0c(mem,index,2);
        }
        *object = mem;
        return 1;
    }
    return 0;
}
