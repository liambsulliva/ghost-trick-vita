struct Vec {
    int x;
    int y;
};
struct Node {
    unsigned char before[0xc4];
    struct Vec pos;
    struct Node *child;
    struct Vec base;
};
extern void func_0203a358(struct Node *node, int x, int y);

void func_02039498(struct Node *node, int x, int y)
{
    node->pos.x = node->base.x + x;
    node->pos.y = node->base.y + y;
    func_0203a358(node->child, x, y);
}
