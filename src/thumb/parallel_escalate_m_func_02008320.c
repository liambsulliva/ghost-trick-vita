typedef struct Type {
    int pad;
    int id;
    int pad2[2];
    struct Type *parent;
} Type;

int func_02008320(void *first, Type *type, Type *kind)
{
    Type *node;

    if (*(int *)((char *)type + 4) == *(int *)((char *)kind + 4)) {
        return 1;
    }
    for (node = type->parent; node != 0; node = node->parent) {
        if (node->id == kind->id) {
            return 1;
        }
    }
    return 0;
}
