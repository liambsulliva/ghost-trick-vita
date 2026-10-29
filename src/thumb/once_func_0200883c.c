extern unsigned int data_020b1910;
extern char data_020b1920;
void func_0202ef88(void *object);

void *func_0200883c(void)
{
    if ((data_020b1910 & 1) == 0) {
        func_0202ef88(&data_020b1920);
        data_020b1910 = data_020b1910 | 1;
    }
    return &data_020b1920;
}
