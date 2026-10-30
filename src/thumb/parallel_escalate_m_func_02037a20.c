struct Self02037a20 {
    char pad[0x55];
    unsigned char ready;
    unsigned char notify;
};

extern unsigned char data_020cdd0c[];
extern char data_0209a86c;
extern char data_0209a870;
extern char data_0209a878;
extern char data_0209a880;
extern char data_020cdd90;
extern char data_020ce7a0;
extern void func_02033140(struct Self02037a20 *self);
extern void *func_02009900(void);
extern void func_0202fad4(void *system, void *name);
extern void func_0200da04(char *name, char *value, char *text, char *extra);
extern int func_0202fb48(void *system, void *kind, int start);
extern void func_02010dc4(void);
extern void func_02038090(struct Self02037a20 *self, unsigned char value);

void func_02037a20(struct Self02037a20 *self)
{
    func_02033140(self);
    if (self->ready == 0) {
        func_0202fad4(func_02009900(), &data_020cdd90);
    }
    if (self->notify != 0) {
        func_0200da04(&data_0209a86c, &data_0209a870, &data_0209a878, &data_0209a880);
    }
    data_020cdd0c[2] = 0;
    if (func_0202fb48(func_02009900(), &data_020ce7a0, -1) != 0) {
        func_02010dc4();
    }
    func_02038090(self, 0);
}
