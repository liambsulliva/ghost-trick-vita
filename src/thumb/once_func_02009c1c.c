extern unsigned int data_020b429c;
extern char data_020b441c;
void func_0202f1a0(void *object);

void *func_02009c1c(void)
{
    if ((data_020b429c & 1) == 0) {
        func_0202f1a0(&data_020b441c);
        data_020b429c = data_020b429c | 1;
    }
    return &data_020b441c;
}
