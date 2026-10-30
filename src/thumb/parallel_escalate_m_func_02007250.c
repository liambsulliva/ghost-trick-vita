struct State {
    unsigned int first;
    unsigned int mode;
    unsigned int value0;
    unsigned int value1;
    unsigned int value2;
    unsigned int value3;
    unsigned int value4;
    unsigned int value5;
};
extern struct State data_020b18d8;
extern char data_020b18dc;
extern void func_0200743c(void *object);

void func_02007250(unsigned short a,unsigned short b,unsigned int c,unsigned int d,unsigned char e,unsigned char f,short g,short h,unsigned char i,short j,unsigned short k,unsigned char l)
{
    data_020b18d8.mode = 5;
    data_020b18d8.value0 = (a << 16) | b;
    data_020b18d8.value1 = c;
    data_020b18d8.value2 = d;
    data_020b18d8.value3 = (l << 24) | ((e & 0xff) << 16) | ((f & 0xff) << 8) | i;
    data_020b18d8.value4 = (j << 16) | k;
    data_020b18d8.value5 = (g << 16) | (h & 0xff);
    func_0200743c(&data_020b18dc);
}
