extern int func_0206c6d8(void *file, int position, int origin);
extern int func_0206c7ec(void *file);
extern int func_0206c704(void *file, unsigned char *buffer, int length);

int func_02000ce8(void *file, unsigned char *buffer, int size, int position)
{
    int remain;
    int chunk;
    int address;
    int read;

    remain = size & 0xfffff;
    chunk = 0;
    if (func_0206c6d8(file, position, 0) == 0) {
        return -3;
    }
    address = func_0206c7ec(file);
    if (remain > 0x200 && address != 0) {
        chunk = ((address + 0x200) & 0xfffffe00) - address;
    } else {
        if (remain > 0x200) {
            chunk = 0x200;
        } else {
            chunk = remain;
        }
    }
    while (remain > 0) {
        read = func_0206c704(file, buffer, chunk);
        if (read != chunk) {
            return -2;
        }
        buffer += read;
        remain -= read;
        if (remain > 0x200) {
            chunk = 0x200;
        } else {
            chunk = remain;
        }
    }
    return size;
}
