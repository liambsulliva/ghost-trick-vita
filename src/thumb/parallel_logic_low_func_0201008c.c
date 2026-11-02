struct Flagged {
    int pad[3];
    unsigned int flag : 1;
};

int func_0201008c(struct Flagged *param_1)
{
    if (param_1->flag == 1) {
        return 1;
    }
    return 0;
}
