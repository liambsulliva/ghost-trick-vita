struct Links {
    unsigned int next;
    unsigned int previous;
};

struct List {
    unsigned int first;
    unsigned int last;
    unsigned short count;
    unsigned short offset;
};

void func_02067f4c(struct List *list,unsigned int object)
{
    struct Links *links;

    links = (struct Links *)(object + list->offset);
    if (links->next == 0) {
        list->first = links->previous;
    }
    else {
        ((struct Links *)(links->next + list->offset))->previous = links->previous;
    }
    if (links->previous == 0) {
        list->last = links->next;
    }
    else {
        ((struct Links *)(links->previous + list->offset))->next = links->next;
    }
    links->next = 0;
    links->previous = 0;
    list->count = list->count - 1;
}
