struct Object02046c5c {
    void *vtable;
    int pad[10];
    int handle;
};

extern char data_020a8adc;
void *func_02008444(void);
int func_02031af4(void *manager, int handle);
void func_02031b04(void *manager, int *handle);
void func_0202f5e4(struct Object02046c5c *object);

struct Object02046c5c *func_02046c5c(struct Object02046c5c *object)
{
    int handle;

    object->vtable = &data_020a8adc;
    handle = object->handle;
    if (handle >= 0) {
        if (func_02031af4(func_02008444(), handle) == 0) {
            func_02031b04(func_02008444(), &object->handle);
        }
    }
    func_0202f5e4(object);
    return object;
}
