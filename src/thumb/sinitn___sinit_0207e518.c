extern char data_020b9ce8;
extern char data_020847f8;
extern char data_020bbc18;
void func_020157f4(void *a, void *b, void *c, int d, int e, int f);
extern char data_020b9c94;
extern char data_02084804;
extern char data_020cb030;
void func_02015810(void *a, void *b, void *c, int d, int e, int f);
extern char data_020b9cb0;
extern char data_02084810;
void func_0201582c(void *a, void *b, void *c, int d, int e, int f);
extern char data_020b9ccc;
extern char data_02084820;
void func_02015848(void *a, void *b, void *c, int d, int e, int f);

void __sinit_0207e518(void)
{
    func_020157f4(&data_020b9ce8, &data_020847f8, &data_020bbc18, 16, 0, 0);
    func_02015810(&data_020b9c94, &data_02084804, &data_020cb030, 84, 0, 0);
    func_0201582c(&data_020b9cb0, &data_02084810, &data_020b9c94, 100, 0, 0);
    func_02015848(&data_020b9ccc, &data_02084820, &data_020b9cb0, 100, 0, 0);
}
