extern unsigned int func_02075c98(void);
extern void func_02075cac(unsigned int bits);

typedef struct {
    char pad[4];
    unsigned int count;
} Counter;

extern Counter data_020d0f0c;

unsigned int func_02074398(void)
{
    unsigned int saved;
    unsigned int count;
    unsigned int old;

    saved = func_02075c98();
    count = data_020d0f0c.count;
    if (count < 0xffffffffU) {
        data_020d0f0c.count = count + 1;
        old = count;
    }
    func_02075cac(saved);
    return old;
}
