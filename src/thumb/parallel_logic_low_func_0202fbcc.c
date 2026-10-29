extern int func_02009900(void);
extern void func_0202fbf8(unsigned int *object,int value);

unsigned int *func_0202fbcc(unsigned int *object,int value)
{
    int alive;

    alive = 1;
    *(unsigned char *)(object + 3) = alive;
    *object = func_02009900();
    if (value == alive - 2) {
        *(unsigned char *)(object + 3) = 0;
        func_0202fbf8(object,0);
    }
    else {
        func_0202fbf8(object,value);
    }
    return object;
}
