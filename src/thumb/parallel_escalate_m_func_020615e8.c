extern unsigned int *func_02008444(void);
extern int func_0201132c(unsigned int *object);
extern int func_02061640(unsigned int *object);
extern int func_020328e4(unsigned int *object);
extern int func_02061654(unsigned int *object);
extern int func_02061668(unsigned int *object);

int func_020615e8(int param_1)
{
    int result;

    result = *(int *)(param_1 + 0xa4);
    if (func_0201132c(func_02008444()) != 0) {
        if (func_02061640(func_02008444()) != 0) {
            result = 0xa8000;
        } else {
            result = 0x70000;
        }
    } else if (func_020328e4(func_02008444()) != 0) {
        if (func_02061654(func_02008444()) != 0) {
            result = 0xa8000;
        } else {
            result = 0x30000;
        }
    } else if (func_02061668(func_02008444()) != 0) {
        result = 0xa8000;
    }
    return result;
}
