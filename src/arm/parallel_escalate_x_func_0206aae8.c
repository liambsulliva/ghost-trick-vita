#pragma opt_arithtransformation off
enum Page { PAGE_SIZE_E = 512, PAGE_BIG = 0x7fffffff };
extern unsigned int data_020b1724;
extern unsigned char data_020d09a0[];

extern void func_0206a948(unsigned int offset);
extern int func_0206a97c(void);
extern void func_0206b168(int value);
extern void func_0207303c(const void *src, void *dst, unsigned int len);

int func_0206aae8(void *userdata, void *buffer, unsigned int offset, unsigned int length)
{
    int retval = (int)length;
    unsigned int cachePage = data_020b1724;

    while (length > 0) {
        unsigned char *ptr = (unsigned char *)buffer;
        enum Page n = PAGE_SIZE_E;
        unsigned int pos = offset & ~(512 - 1);
        if (pos == cachePage) {
            ptr = data_020d09a0;
        } else {
            if (pos != offset || ((unsigned int)buffer & 3) != 0 || length < n) {
                ptr = data_020d09a0;
                cachePage = pos;
            }
            func_0206a948(pos);
            {
                unsigned int word = 0;
                for (;;) {
                    unsigned int ctrl = *(volatile unsigned int *)0x040001a4;
                    if ((ctrl & 0x00800000) != 0) {
                        unsigned int data = *(volatile unsigned int *)0x04100010;
                        if (word < 512 / sizeof(unsigned int)) {
                            ((unsigned int *)ptr)[word++] = data;
                        }
                    }
                    if ((ctrl & 0x80000000) == 0) {
                        break;
                    }
                }
            }
        }
        if (ptr == data_020d09a0) {
            unsigned int mod = offset - pos;
            n = (length <= 512 - mod) ? length : 512 - mod;
            func_0207303c(data_020d09a0 + mod, buffer, n);
        }
        buffer = (unsigned char *)buffer + n;
        offset += n;
        length -= n;
    }
    func_0206b168(func_0206a97c());
    data_020b1724 = cachePage;
    return retval;
}
