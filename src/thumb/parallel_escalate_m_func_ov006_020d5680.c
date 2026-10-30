struct Item {
    unsigned int header;
    unsigned int value;
    unsigned char a, b, c, d;
};

void func_ov006_020d5680(struct Item *__restrict p, struct Item *__restrict q)
{
    p->value = q->value;
    p->a = q->a;
    p->b = q->b;
    p->c = q->c;
    p->d = q->d;
}
