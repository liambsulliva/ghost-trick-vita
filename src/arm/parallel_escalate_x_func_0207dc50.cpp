extern "C" {
extern void *func_02031288(unsigned int size);
extern void func_020312a4(void *block);
extern void func_0207dbf4(void *array, void *end, unsigned int size, void (*dtor)(void *));

void *func_0207dc50(unsigned int count, unsigned int size, unsigned int header, void (*ctor)(void *), void (*dtor)(void *))
{
    char *block;
    char *p;
    unsigned int *tail;

    block = (char *)func_02031288(count * size + header);
    if (block == 0) {
        return 0;
    }
    if (header != 0) {
        tail = (unsigned int *)(block + header);
        tail[-1] = count;
        if (header >= 8) {
            tail[-2] = size;
        }
    }
    if (ctor != 0) {
        try {
            p = block + header;
            while (count != 0) {
                ctor(p);
                p += size;
                count--;
            }
        } catch (...) {
            if (dtor != 0) {
                func_0207dbf4(block + header, p, size, dtor);
            }
            func_020312a4(block);
            throw;
        }
    }
    return block + header;
}
}
