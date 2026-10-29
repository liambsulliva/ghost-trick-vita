extern unsigned int data_020b89a0;
extern char data_020b8a68;
void func_0202f01c(void *object);

void *func_02010b7c(void)
{
    if ((data_020b89a0 & 1) == 0) {
        func_0202f01c(&data_020b8a68);
        data_020b89a0 = data_020b89a0 | 1;
    }
    return &data_020b8a68;
}
