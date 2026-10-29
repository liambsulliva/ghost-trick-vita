struct Pair {
    unsigned int first;
    unsigned int second;
};

struct Record {
    unsigned int first;
    struct Pair rest;
};

void func_0201ed80(struct Record *destination,struct Record *source)
{
    destination->first = source->first;
    destination->rest = source->rest;
}
