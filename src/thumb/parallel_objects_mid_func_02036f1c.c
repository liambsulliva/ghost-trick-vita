struct Flagged {
    int pad[3];
    unsigned int low : 23;
    unsigned int flag : 1;
};

int func_02036f1c(struct Flagged *param_1)
{
    if (param_1->flag == 0) {
        return 1;
    }
    return 0;
}
