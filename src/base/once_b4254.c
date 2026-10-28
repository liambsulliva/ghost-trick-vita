extern unsigned int data_020b4250;
extern char data_020b4254;
void func_02020df4(void *object);

void *func_02008eb0(void)
{
    if ((data_020b4250 & 1) == 0) {
        func_02020df4(&data_020b4254);
        data_020b4250 = data_020b4250 | 1;
    }
    return &data_020b4254;
}
