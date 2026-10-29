extern void func_0202fbcc(int *stack,int start);
extern int func_0200eac4(int *stack);
extern int func_0200eab4(int *stack);
extern int func_02008828(int object,int kind);
extern int func_0200eab8(int *stack);
extern void func_0200eabc(int *stack,int value);

int func_0202fb48(int unused,int kind,int start)
{
    int stack[4];

    func_0202fbcc(stack,start);
    while (func_0200eac4(stack) != 0) {
        if (func_02008828(func_0200eab4(stack),kind) != 0) {
            return func_0200eab8(stack);
        }
        func_0200eabc(stack,0);
    }
    return 0;
}
