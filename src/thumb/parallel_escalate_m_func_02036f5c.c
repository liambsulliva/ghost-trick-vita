struct State02036f5c {
    char pad[4];
    unsigned char mode;
    char pad05[0x27];
    int active;
    char pad30[4];
    unsigned char busy;
};

extern char data_0209a724;
extern char data_0209a728;
extern char data_0209a730;
extern char data_0209a738;
extern void func_0200da04(char *name, char *value, char *text, char *extra);
extern void *func_02008444(void);
extern void func_02010ef4(void *object, int value, int extra);
extern void func_02010f0c(void *object, int value, int extra);

void func_02036f5c(struct State02036f5c *self)
{
    if (self->active != 0) {
        func_0200da04(&data_0209a724, &data_0209a728, &data_0209a730, &data_0209a738);
    }
    switch (self->mode) {
    case 2:
        func_02010ef4(func_02008444(), 0, -1);
        break;
    case 5:
        func_02010ef4(func_02008444(), 0xff, -1);
        break;
    case 1:
    case 3:
    case 4:
        break;
    default:
        if (self->busy == 0) {
            func_02010f0c(func_02008444(), 1, 0);
        }
        break;
    }
}
