struct Flagged {
    int pad[3];
    unsigned int low : 23;
    unsigned int flag : 1;
};

void func_02014230(struct Flagged *p, int value)
{
    p->flag = (value == 0);
}
