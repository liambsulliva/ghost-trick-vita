extern "C" {
extern void func_0207dbf4(void *array, void *end, unsigned int size, void (*dtor)(void *));

void func_0207dd08(void *array, unsigned int count, unsigned int size, void (*ctor)(void *), void (*dtor)(void *))
{
    char *p;

    if (ctor == 0) {
        return;
    }
    if (dtor != 0) {
        p = (char *)array;
        try {
            while (count != 0) {
                ctor(p);
                p += size;
                count--;
            }
        } catch (...) {
            func_0207dbf4(array, p, size, dtor);
            throw;
        }
    } else {
        while (count != 0) {
            ctor(array);
            array = (char *)array + size;
            count--;
        }
    }
}
}
