extern char data_0209c1f4[];
extern char data_0209c1fc[];
extern char *data_0209c0d0[];
extern unsigned char data_020cff60;
extern char data_020ceec0[];
extern char *func_02078e84(char *dest, const char *src);
extern char *func_02078fbc(char *text, const char *part);
extern char *func_020790a4(char *dest, const char *src);
void func_02051dc4(const char *name, int suffix)
{
    char buffer[128];
    char *found;
    func_02078e84(buffer, name);
    found = func_02078fbc(buffer, data_0209c1f4);
    if (found != 0) {
        *found = 0;
    }
    if (suffix != 0) {
        func_020790a4(buffer, data_0209c0d0[data_020cff60]);
    }
    func_02078e84(data_020ceec0, buffer);
    func_020790a4(data_020ceec0, data_0209c1f4);
    func_020790a4(data_020ceec0, data_0209c1fc);
}
