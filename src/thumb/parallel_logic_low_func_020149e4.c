extern unsigned int *func_0201003c(void);
extern int func_02015990(int node,int flag);
extern void func_020152a4(int node,int a,int b,int c,int d);

void func_020149e4(int object,int a,int b)
{
    int self;
    int node;
    int zero;

    self = object;
    node = *(int *)(self + 4);
    if (node != 0) {
        func_02015990((int)func_0201003c(),1);
        zero = 0;
        func_020152a4(*(int *)(self + 4),a,b,zero,zero);
    }
}
