extern int func_02031254(int size);
extern int func_02020ec4(int param_1);
extern char *func_02008444(void);
extern int func_02021788(int param_1, int *param_2, int param_3, char *param_4, int param_5);
extern void func_02020ec8(int param_1);
extern void func_02031270(int param_1);

void func_020119d0(int *param_1, int *param_2)
{
    int result;

    result = func_02031254(0x1c);
    if (result != 0) {
        result = func_02020ec4(result);
    }
    if (result != 0) {
        func_02021788(result, param_2, 0xd, func_02008444() + 4, 0);
    }
    if (result != 0) {
        func_02020ec8(result);
        func_02031270(result);
    }
}
