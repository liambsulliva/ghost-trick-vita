extern int func_020781f4(void *data,int length,unsigned int flag,void (*callback)(),unsigned int *result);
extern void func_02075d14(unsigned int count);
extern void func_02078068();

unsigned int func_02078278(void *data,int length)
{
    int result;

    for (;;) {
        result = (int)0xffff0000;
        while (func_020781f4(data,length,0,func_02078068,(unsigned int *)&result) != 0) {
            func_02075d14(0x51d23);
        }
        while (*(unsigned int *)&result == 0xffff0000) {
            func_02075d14(0x51d23);
        }
        if (result == 0) {
            return result;
        }
        func_02075d14(0x51d23);
    }
}
