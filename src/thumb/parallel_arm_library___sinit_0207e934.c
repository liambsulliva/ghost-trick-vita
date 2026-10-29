extern char data_020ca04c[0x30];
extern char data_02085164;
extern char data_020bbc18;
extern char data_020ca00c;
extern char data_020ca07c;
extern char data_020c9fd0;
void func_02028a70(void *a, void *b, void *c, int d, int e, int f);
void func_02022310(void *a);
void func_0207dd08(void *a, int b, int c, void *d, void *e);
void func_0207da20(int a, int b, void *c);
void func_020223fc(void);
void func_02028a8c(void);

void __sinit_0207e934(void)
{
    func_02028a70(&data_020ca04c, &data_02085164, &data_020bbc18, sizeof(data_020ca04c), 0, 0);
    func_02022310(&data_020ca00c);
    func_0207dd08(&data_020ca07c, 4, 0x10, (void *)func_02022310, (void *)func_020223fc);
    func_0207da20(0, (int)((unsigned int)func_02028a8c), &data_020c9fd0);
}
