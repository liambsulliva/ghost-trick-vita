typedef struct {
    int head;
    int count;
} Pool02020d10;

extern Pool02020d10 data_020bbd24;
extern char data_020bbd38[];

int func_02020d10(void)
{
    int head;
    int next;
    int index;

    head = data_020bbd24.head;
    if (head != 0) {
        next = *(int *)(head + 0x34);
        if (next != 0) {
            data_020bbd24.head = next;
        }
        else {
            data_020bbd24.head = 0;
        }
        return head;
    }
    index = data_020bbd24.count;
    data_020bbd24.count = index + 1;
    return (int)(data_020bbd38 + index * 0x38);
}
