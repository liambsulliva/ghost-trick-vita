class Node {
public:
    virtual void s0();
    virtual void s1();
    unsigned char level;
    unsigned char rank;
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 1;
    unsigned char b7 : 1;
    unsigned char owner_byte;
    Node *next;
};
extern "C" {
Node *func_0201f7f0(Node *owner);
int func_02008444(void);
void func_0201f7e4(int object);

Node *func_0201f6fc(Node *node, unsigned char level, unsigned char rank)
{
    Node *result;
    Node *child;
    result = 0;
    if (node->b3 == 0) {
        result = node;
        goto apply;
    }
    if (node->b7) {
        if (node->rank == rank) {
            result = node;
            node->b1 = 1;
        } else {
            result = func_0201f7f0(node);
            if (result == 0) {
                goto mark;
            }
            result->b1 = 1;
            result->b6 = 1;
        }
        node->b7 = 0;
    mark:
        func_0201f7e4(func_02008444());
        goto apply;
    }
    if (node->level >= level) {
        return node;
    }
    if (node->b6) {
        child = node->next;
        if (child == 0) {
            goto apply;
        }
        if (child->rank == rank) {
            node->b6 = 0;
            result = node;
            node->next = child->next;
            child->next = 0;
            if (child != 0) {
                child->s1();
            }
            goto apply;
        }
        if (child->rank < rank) {
            node->b6 = 0;
            result = node;
        } else {
            result = node;
        }
    } else {
        if (node->rank < rank) {
            result = func_0201f7f0(node);
            if (result != 0) {
                result->b1 = node->b1;
                goto apply;
            }
            return node;
        }
        if (node->rank != rank) {
            goto apply;
        }
        result = node;
    }
apply:
    if (result != 0) {
        result->level = level;
        result->rank = rank;
        result->b2 = 1;
        result->b3 = 1;
        result->b7 = 0;
    }
    return result;
}
}
