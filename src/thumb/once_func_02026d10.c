extern unsigned int data_020ca068;
extern char data_020ca06c;
void func_0202fbb0(void *object);

void *func_02026d10(void)
{
    if ((data_020ca068 & 1) == 0) {
        func_0202fbb0(&data_020ca06c);
        data_020ca068 = data_020ca068 | 1;
    }
    return &data_020ca06c;
}
