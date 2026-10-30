extern void func_0201ddb4(void *object,unsigned int index,int count);
extern int data_020a450c;

void *func_0201df0c(unsigned int *object,unsigned int index,int count)
{
    unsigned int i;
    unsigned int next;

    func_0201ddb4(object,index,count);
    *object = (unsigned int)&data_020a450c;
    i = 0;
    do {
        *(unsigned short *)((char *)object + i * 0x1c + 0x36) = 0;
        *(unsigned char *)((char *)object + i * 0x1c + 0x38) = 0;
        *(unsigned char *)((char *)object + i * 0x1c + 0x39) = 0;
        *(unsigned short *)((char *)object + i * 0x1c + 0x3a) = 0;
        next = i + 1;
        *(unsigned short *)((char *)object + i * 0x1c + 0x3c) = 0;
        i = next;
    } while (i < 3);
    return object;
}
