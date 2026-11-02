struct Flagged {
    int pad[4];
    unsigned int low : 25;
    unsigned int flag : 1;
};

int func_0205d0c0(struct Flagged *param_1)
{
    if (param_1->flag == 1) {
        return 1;
    }
    return 0;
}
