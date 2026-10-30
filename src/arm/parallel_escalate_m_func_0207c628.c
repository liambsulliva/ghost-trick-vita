extern void func_0207c5c4(unsigned char *);
extern void func_0207c5c8(unsigned char *);

int func_0207c628(unsigned int param_1, unsigned char *buf, unsigned int *len)
{
    unsigned char saved;
    unsigned char *end;

    if (*len > 1) {
        end = buf + *len;
        saved = end[-1];
        end[-1] = 0;
        func_0207c5c8(buf);
        func_0207c5c4(&saved);
    } else {
        func_0207c5c4(buf);
    }
    return 0;
}
