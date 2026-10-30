typedef struct {
    unsigned int *head;
    int unk4;
    int unk8;
    int unkc;
    unsigned int *tail;
} Info02076f9c;

extern Info02076f9c data_020d1344;
extern int func_02075c98(void);
extern void func_02075cac(int);

unsigned int *func_02076f9c(void)
{
    int enabled;
    unsigned int *node;
    unsigned int *next;

    enabled = func_02075c98();
    node = data_020d1344.head;
    if (node == 0) {
        func_02075cac(enabled);
        return 0;
    }
    next = (unsigned int *)*node;
    data_020d1344.head = next;
    if (next == 0) {
        data_020d1344.tail = 0;
    }
    func_02075cac(enabled);
    return node;
}
