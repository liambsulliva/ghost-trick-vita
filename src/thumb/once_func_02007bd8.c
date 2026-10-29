extern unsigned int data_020b190c;
extern char data_020b1960;
void func_0201f188(void *object);

void *func_02007bd8(void)
{
    if ((data_020b190c & 1) == 0) {
        func_0201f188(&data_020b1960);
        data_020b190c = data_020b190c | 1;
    }
    return &data_020b1960;
}
