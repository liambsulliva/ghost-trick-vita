extern char data_020cf36c[0x154];
extern char data_020cf860;
extern char data_0209d644;
extern char data_020cf3f8[0x154];
extern char data_0209d650;
extern char data_020cf3dc[0x154];
extern char data_0209d65c;
extern char data_020cf388[0x15c];
extern char data_0209d66c;
extern char data_020cf3a4[0x15c];
extern char data_0209d680;
extern char data_020cf3c0[0x15c];
extern char data_0209d690;
void func_02053f90(void *a, void *b, void *c, int d, int e, int f);
void func_02053fac(void *a, void *b, void *c, int d, int e, int f);
void func_02053fc8(void *a, void *b, void *c, int d, int e, int f);
void func_02053fe4(void *a, void *b, void *c, int d, int e, int f);
void func_02054000(void *a, void *b, void *c, int d, int e, int f);
void func_0205401c(void *a, void *b, void *c, int d, int e, int f);

void __sinit_02080374(void)
{
    unsigned char *first;
    unsigned char *shared;
    unsigned char *fourth;

    first = (unsigned char *)&data_020cf36c;
    shared = (unsigned char *)&data_020cf860;
    func_02053f90(first, &data_0209d644, shared, sizeof(data_020cf36c), 0, 0);
    func_02053fac(&data_020cf3f8, &data_0209d650, first, sizeof(data_020cf3f8), 0, 0);
    func_02053fc8(&data_020cf3dc, &data_0209d65c, first, sizeof(data_020cf3dc), 0, 0);
    fourth = (unsigned char *)&data_020cf388;
    func_02053fe4(fourth, &data_0209d66c, shared, sizeof(data_020cf388), 0, 0);
    func_02054000(&data_020cf3a4, &data_0209d680, fourth, sizeof(data_020cf3a4), 0, 0);
    func_0205401c(&data_020cf3c0, &data_0209d690, fourth, sizeof(data_020cf3c0), 0, 0);
}
