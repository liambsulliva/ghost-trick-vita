extern unsigned int data_020b89ac;
extern char data_020b8ad4;
void func_02023240(void *object);

void *func_02011264(void)
{
    if ((data_020b89ac & 1) == 0) {
        func_02023240(&data_020b8ad4);
        data_020b89ac = data_020b89ac | 1;
    }
    return &data_020b8ad4;
}
