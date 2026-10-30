struct Field;

struct Args {
    struct Field *object;
};

extern int func_0201ee68(struct Field *object);

int func_02031240(struct Args args, ...)
{
    return func_0201ee68(args.object);
}
