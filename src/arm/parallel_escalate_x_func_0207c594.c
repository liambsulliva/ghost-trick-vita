void *func_0207c594(void *dest, unsigned int count)
{
    unsigned char *p = dest;
    if (dest != 0 && count != 0) {
        do {
            *p++ = 0;
        } while (--count != 0);
    }
    return dest;
}
