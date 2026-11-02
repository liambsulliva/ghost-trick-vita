struct Task;

struct Bucket {
    int unknown_0;
    struct Task *tail;
    struct Task *head;
    int unknown_c;
};

struct Manager {
    unsigned char unknown_0[0x24];
    struct Bucket buckets[16];
};

struct Task {
    int unknown_0;
    struct Task *prev;
    struct Task *next;
    unsigned char unknown_c;
    unsigned char priority;
};

void func_0202fa50(struct Manager *m, struct Task *task)
{
    struct Bucket *bucket;
    struct Task *next;
    struct Task *prev;

    bucket = &m->buckets[task->priority];
    next = task->next;
    prev = task->prev;
    if (next == 0) {
        bucket->tail = prev;
        if (task->prev != 0) {
            bucket->tail->next = 0;
            return;
        }
    } else {
        if (prev == 0) {
            bucket->head = next;
            next->prev = 0;
            return;
        }
        next->prev = prev;
        task->prev->next = task->next;
        task->next = 0;
    }
}
