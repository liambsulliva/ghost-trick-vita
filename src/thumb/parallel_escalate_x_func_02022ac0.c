struct Sprite {
    char pad[0x42];
    unsigned short priority;
};

struct SpriteList {
    unsigned short count;
    char pad[6];
    struct Sprite *items[0x280];
    unsigned short order[1];
};

void func_02022ac0(struct SpriteList *list, struct Sprite *sprite)
{
    unsigned int i;
    unsigned int j;

    list->items[list->count] = sprite;
    list->order[list->count] = list->count;
    for (i = 0; i < list->count; i++) {
        if (list->items[list->order[i]]->priority < list->items[list->order[list->count]]->priority) {
            for (j = list->count; j > i; j--) {
                list->order[j] = list->order[j - 1];
            }
            list->order[i] = list->count;
            break;
        }
    }
    list->count++;
}
