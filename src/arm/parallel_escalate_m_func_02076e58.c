extern unsigned int *data_020d1344;
extern int func_02075c98(void);
extern void func_02075cac(int);

int func_02076e58(void)
{
    int enabled;
    unsigned int *node;
    int count;

    enabled = func_02075c98();
    count = 0;
    node = data_020d1344;
    if (node != 0) {
        do {
            node = (unsigned int *)*node;
            count = count + 1;
        } while (node != 0);
    }
    func_02075cac(enabled);
    return count;
}
