struct Flagged {
    int pad[4];
    unsigned int low : 20;
    unsigned int flag : 1;
};

int func_0205fa18(struct Flagged *param_1)
{
    if (param_1->flag == 1) {
        return 1;
    }
    return 0;
}
