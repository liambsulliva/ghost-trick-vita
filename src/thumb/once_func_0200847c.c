extern unsigned int data_020b1918;
extern char data_020b1994;
void func_02029dcc(void *object);

void *func_0200847c(void)
{
    if ((data_020b1918 & 1) == 0) {
        func_02029dcc(&data_020b1994);
        data_020b1918 = data_020b1918 | 1;
    }
    return &data_020b1994;
}
