struct Node {
    unsigned int first;
    unsigned int second;
    unsigned int third;
    unsigned int fourth;
    unsigned int fifth;
};
struct Pool {
    char *start;
    struct Node *node;
};
extern struct Pool data_020b18fc;
extern char *data_020b1908;
extern void func_02007420(char *start,unsigned int size);
void func_020073dc(void)
{
    struct Node *node;
    node = (struct Node *)(data_020b1908 + 0x4d40);
    data_020b18fc.node = node;
    data_020b18fc.start = (char *)node + 0x14;
    node->second = 0;
    data_020b18fc.node->first = data_020b18fc.node->second;
    data_020b18fc.node->fourth = 0;
    data_020b18fc.node->fifth = 1;
    data_020b18fc.node->third = 0;
    func_02007420(data_020b18fc.start,0x400);
}
