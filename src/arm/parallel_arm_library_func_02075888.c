struct State {
    unsigned short flag;
    unsigned short pad;
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
};
extern struct State data_020d126c;
extern void func_020735f0(int);

void func_02075888(void)
{
    if (data_020d126c.flag == 0) {
        data_020d126c.flag = 1;
        data_020d126c.c = 0;
        data_020d126c.d = 0;
        func_020735f0(4);
        data_020d126c.b = 0;
        data_020d126c.a = 0;
    }
}
