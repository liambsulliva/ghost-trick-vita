struct Entry {
    unsigned short *ids;
    int extra;
};
struct State {
    unsigned int before_flags[21];
    unsigned int flags;
};
struct Self {
    int head;
    unsigned char mode;
    unsigned char busy;
    unsigned char pad[26];
    int index;
    unsigned char middle[40];
    int link;
};
extern const struct Entry data_0208332c[];
extern struct State data_020c9e48;
extern void func_0203f21c(struct Self *object);
extern int func_0203f254(struct Self *object, unsigned int id);

void func_0203f0ec(struct Self *param_1, int param_2)
{
    unsigned short *ids;

    if (param_1->index == 0) {
        func_0203f21c(param_1);
    }
    ids = data_0208332c[param_2].ids;
    param_1->index = param_2;
    if (func_0203f254(param_1, *ids) != 0) {
        data_020c9e48.flags = data_020c9e48.flags | 0x800;
    }
    param_1->mode = 2;
    if (param_1->link != 0) {
        param_1->busy = 1;
    }
}
