extern unsigned int data_020ca4dc;
extern char data_020ca4e0;
void func_0201dcd8(void *object);

void *func_0202f25c(void)
{
    if ((data_020ca4dc & 1) == 0) {
        func_0201dcd8(&data_020ca4e0);
        data_020ca4dc = data_020ca4dc | 1;
    }
    return &data_020ca4e0;
}
