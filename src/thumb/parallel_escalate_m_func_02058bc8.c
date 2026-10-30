typedef struct { unsigned char pad[0xbc]; short a; unsigned char b; signed char c; short d; unsigned char e; unsigned char f; } Fields;
extern void func_0202fe34();
extern void func_0200f70c();
extern void func_020083b0();
extern char data_020ae600;
extern char data_0209d958;
extern char data_020a46c8;

unsigned int *func_02058bc8(unsigned int *param_1)
{
    unsigned int i;
    unsigned char *elem;

    func_0202fe34(param_1);
    *param_1 = (unsigned int)&data_020ae600;
    ((Fields *)param_1)->a = -1;
    ((Fields *)param_1)->b = 0;
    ((Fields *)param_1)->c = -1;
    ((Fields *)param_1)->d = 0;
    ((Fields *)param_1)->e = 0;
    ((Fields *)param_1)->f = 0;
    *(unsigned int *)((unsigned char *)param_1 + 0xd4) = 0;
    *(unsigned int *)((unsigned char *)param_1 + 0xd8) = 0;
    *(unsigned int *)((unsigned char *)param_1 + 0xdc) = 0;
    *(unsigned int *)((unsigned char *)param_1 + 0xe0) = (unsigned int)&data_0209d958;
    elem = (unsigned char *)param_1 + 0xe4;
    do {
        func_0200f70c(elem);
        elem = elem + 0xc;
    } while (elem < (unsigned char *)param_1 + 0x114);
    for (i = 0; i < 4; i++) {
        *(unsigned int *)((unsigned char *)param_1 + 0xc4 + i * 4) = 0;
    }
    for (i = 0; i < 4; i++) {
        func_020083b0((unsigned char *)param_1 + 0xe4 + i * 12, &data_020a46c8);
    }
    return param_1;
}
