extern char data_020ca128;
extern char data_0208517c;
extern char data_020cf708;
void func_02029890(void *a, void *b, void *c, int d, int e, int f);
extern char data_020ca0f0;
extern char data_0208518c;
void func_020298ac(void *a, void *b, void *c, int d, int e, int f);
extern char data_020ca10c;
extern char data_020851a4;
void func_020298c8(void *a, void *b, void *c, int d, int e, int f);

void __sinit_0207e990(void)
{
    func_02029890(&data_020ca128, &data_0208517c, &data_020cf708, 96, 0, 0);
    func_020298ac(&data_020ca0f0, &data_0208518c, &data_020ca128, 96, 0, 0);
    func_020298c8(&data_020ca10c, &data_020851a4, &data_020ca128, 96, 0, 0);
}
