struct Request {
    void *work;
    unsigned int command;
    unsigned int arg;
    unsigned int mask;
    unsigned int a;
    unsigned int b;
    unsigned int c;
};

extern struct Request data_020b18d8;
extern unsigned char data_020b18dc[];
extern int func_0200743c(void *request);

void func_02007150(int id, int sub, unsigned int a, unsigned int b, unsigned char c)
{
    data_020b18d8.mask = -1;
    data_020b18d8.command = 0xf;
    data_020b18d8.arg = (id << 16) | (unsigned short)sub;
    data_020b18d8.mask <<= 16;
    data_020b18d8.a = a;
    data_020b18d8.b = b;
    data_020b18d8.c = c;
    func_0200743c(data_020b18dc);
}
