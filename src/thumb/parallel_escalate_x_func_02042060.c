typedef struct Node42060 {
    int pad0;
    int count;
    unsigned int low : 24;
    unsigned int mid : 7;
    unsigned int flag : 1;
    int next;
    int prev;
    int data;
} Node42060;

void func_02042060(Node42060 *node)
{
    node->count = 0;
    node->low = 0;
    node->flag = 0;
    node->next = 0;
    node->prev = 0;
    node->data = 0;
}
