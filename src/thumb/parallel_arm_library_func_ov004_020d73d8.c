extern unsigned char data_020cf75c[];
extern unsigned char *func_02009900(void);
extern int func_0202fb48(int, int, int);
extern int func_02056964(unsigned char *);
extern void func_0202f7e8(int, int, int, int);
extern void func_0200ee34(int, int, int, int);
extern void func_02056d68(int, int, int);

void func_ov004_020d73d8(void)
{
    int value;

    value = func_0202fb48((int)func_02009900(), (int)data_020cf75c, -1);
    if (value == 0) {
        value = func_02056964(data_020cf75c);
        func_0202f7e8((int)func_02009900(), value, 13, 0);
    }
    if (value != 0) {
        func_0200ee34(value, 31, 31, 31);
        func_02056d68(value, 0, 31);
    }
}
