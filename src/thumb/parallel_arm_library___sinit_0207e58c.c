extern char data_020b9dbc[200];
extern char data_02084d48;
extern char data_020cf724;
extern char data_020b9d84[92];
extern char data_02084d60;
extern char data_020cb030;
extern char data_020b9dd8[92];
extern char data_02084d0c;
extern char data_020b9da0[92];
extern char data_02084cfc;
extern char data_020b9d6c;
extern char data_020b9d28;
extern char data_020b9d58;
extern char data_020b9d4c;
extern char data_020b9df4;
void func_0201d658(void *a, void *b, void *c, int d, int e, int f);
void func_0201d674(void *a, void *b, void *c, int d, int e, int f);
void func_0201d690(void *a, void *b, void *c, int d, int e, int f);
void func_0201d6ac(void *a, void *b, void *c, int d, int e, int f);
void func_0200db74(void *a);
void func_0207da20(int a, int b, void *c);
void func_02015d34(void *a);
void func_0201c568(void *a);
void func_0200db8c(void);
void func_02015d44(void);

void __sinit_0207e58c(void)
{
    unsigned char *first;

    func_0201d658(&data_020b9dbc, &data_02084d48, &data_020cf724, sizeof(data_020b9dbc), 0, 0);
    first = (unsigned char *)&data_020b9d84;
    func_0201d674(first, &data_02084d60, &data_020cb030, sizeof(data_020b9d84), 0, 0);
    func_0201d690(&data_020b9dd8, &data_02084d0c, first, sizeof(data_020b9dd8), 0, 0);
    func_0201d6ac(&data_020b9da0, &data_02084cfc, first, sizeof(data_020b9da0), 0, 0);
    func_0200db74(&data_020b9d6c);
    func_0207da20((int)&data_020b9d6c, (int)((unsigned int)func_0200db8c), &data_020b9d28);
    func_02015d34(&data_020b9d58);
    func_0207da20((int)&data_020b9d58, (int)((unsigned int)func_02015d44), &data_020b9d4c);
    func_0201c568(&data_020b9df4);
}
