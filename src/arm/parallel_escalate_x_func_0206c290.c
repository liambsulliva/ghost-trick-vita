extern unsigned int func_02075c98(void);
extern void func_02075cac(unsigned int intr);
extern void func_020740d0(unsigned int *queue);
extern int func_0206b958(unsigned int *file);

static inline int is_busy(volatile const unsigned int *file)
{
    return (file[3] & 1) ? 1 : 0;
}

static inline int is_succeeded(volatile const unsigned int *file)
{
    return (file[5] == 0) ? 1 : 0;
}

int func_0206c290(unsigned int *file)
{
    int owner = 0;
    unsigned int intr = func_02075c98();

    if (is_busy(file))
    {
        owner = !(file[3] & 0x44);
        if (owner)
        {
            file[3] |= 4;
        }
    }
    func_02075cac(intr);
    if (owner)
    {
        func_0206b958(file);
    }
    else
    {
        intr = func_02075c98();
        while (file[3] & 1)
        {
            func_020740d0(file + 6);
        }
        func_02075cac(intr);
    }
    return is_succeeded(file);
}
