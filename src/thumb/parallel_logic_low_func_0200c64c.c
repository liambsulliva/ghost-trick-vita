extern int func_0200c67c(int value);
extern void func_0200bd94(int object,int value);

void func_0200c64c(int object,int value)
{
    int kind;

    kind = func_0200c67c(value);
    switch (kind) {
    case 0:
        func_0200bd94(object,value);
        break;
    case 1:
        func_0200bd94(object + 0xa88,value);
        break;
    }
}
