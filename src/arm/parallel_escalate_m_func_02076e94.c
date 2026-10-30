typedef struct {
    unsigned int *unk0;
    unsigned int *unk4;
    unsigned int *unk8;
} Info02076e94;

extern Info02076e94 data_020d1344;
extern int func_02075c98(void);
extern void func_02075cac(int);

int func_02076e94(void)
{
    int enabled;
    unsigned int *node;
    int count;

    enabled = func_02075c98();
    count = 0;
    node = data_020d1344.unk8;
    if (node != 0) {
        do {
            node = (unsigned int *)*node;
            count = count + 1;
        } while (node != 0);
    }
    func_02075cac(enabled);
    return count;
}
