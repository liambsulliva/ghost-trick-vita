extern "C" {
extern char data_02083c0c[], data_02083c14[];
extern void *data_020b87a0[];
int func_020790c8(const signed char *, const char *);
void *func_02008444();
void func_02012400(void *, int, signed char *);
void *func_0200d8a8(int);
void *func_0200d7e4(void *);
void func_0200d80c(void *, int);
void func_0200d818(void *, signed char *);
char *func_0200daac(void *);
int func_0200d88c(char *, signed char *);
void func_0205145c(void *, void *);
}
struct Object { virtual void first(); virtual void destroy(); };
extern "C" void func_0200da04(signed char *name, int value, signed char *text)
{
    if (func_020790c8(name, data_02083c0c) == 0) {
        func_02012400(func_02008444(), value, text);
        return;
    }
    unsigned int i;
    Object *object = (Object *)func_0200d8a8(16);
    if (object != 0) object = (Object *)func_0200d7e4(object);
    if (object != 0) {
        if (object != 0) {
            func_0200d80c(object, value);
            if (*text != 0) func_0200d818(object, text);
            else func_0200d818(object, (signed char *)data_02083c14);
            int all = 0;
            if (*name == '*') all = 1;
            for (i = 0; i < 128; ++i) {
                void *item = data_020b87a0[i];
                if (item != 0 && (all || func_0200d88c(func_0200daac(item), name) == 0))
                    func_0205145c(item, object);
            }
        }
        if (object != 0) object->destroy();
    }
}
