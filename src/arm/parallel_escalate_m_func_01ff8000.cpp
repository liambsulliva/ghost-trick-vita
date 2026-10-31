extern "C" {
extern int func_0206c7ec(int file);
extern int func_0206c704(int file, int buf, int len);

int func_01ff8000(int file, int buf, int size)
{
    int len;
    int total;
    int pos;
    int chunk;
    int n;

    len = size & 0xfffff;
    total = 0;
    pos = func_0206c7ec(file);
    if (len > 0x200 && ((pos != 0) & 0x1ff)) {
        chunk = ((pos + 0x200) & ~0x1ff) - pos;
    } else {
        chunk = 0x20000;
        if (len <= 0x20000) {
            chunk = len;
        }
    }
    while (len > 0) {
        n = func_0206c704(file, buf, chunk);
        total += n;
        if (n != chunk) {
            return 0;
        }
        len -= n;
        buf += n;
        chunk = 0x20000;
        if (len <= 0x20000) {
            chunk = len;
        }
    }
    return total;
}
}
