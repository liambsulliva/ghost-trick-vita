extern char data_02083e50[];
extern char data_02083e60[];
extern char data_02083e6c[];
extern char *func_02011264(void);
extern char *func_0201123c(char *base, int index);
extern int func_02022fa4(char *object, char *name);
extern void *func_02009900(void);
extern int func_0202faa4(void *object, char *name);

int func_02011644(void)
{
    int result;

    if (func_02022fa4(func_0201123c(func_02011264(), 1), data_02083e50) != 0) {
        return 1;
    }
    if (func_02022fa4(func_0201123c(func_02011264(), 1), data_02083e60) != 0) {
        return 1;
    }
    result = func_0202faa4(func_02009900(), data_02083e6c);
    return result;
}
