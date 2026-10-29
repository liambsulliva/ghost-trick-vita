extern int func_0200847c(void);
extern void func_0202a0c0(int object,unsigned int value);

void func_020187bc(int object)
{
    unsigned char *flag;
    unsigned short *slot;
    int self;

    self = object;
    flag = (unsigned char *)self;
    flag = flag + 0x90;
    if (*flag == 0) {
        slot = (unsigned short *)self;
        slot = (unsigned short *)((char *)slot + 0x9c);
        func_0202a0c0(func_0200847c(),*slot);
        slot = (unsigned short *)self;
        slot = (unsigned short *)((char *)slot + 0x9c);
        *slot = 5;
        return;
    }
    flag = (unsigned char *)self;
    flag = flag + 0x90;
    *flag = 0;
}
