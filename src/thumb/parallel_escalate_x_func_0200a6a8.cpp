class Object0200a6a8 {
public:
    virtual ~Object0200a6a8();
};

typedef struct State0200a6a8 {
    Object0200a6a8 *object;
    unsigned char mode;
    unsigned char on : 1;
    unsigned char next_on : 1;
    unsigned char rest : 6;
    signed char level;
    signed char next_level;
    unsigned short time;
    unsigned short next_time;
} State0200a6a8;

extern "C" {
void func_0200a6a8(State0200a6a8 *p);
}

void func_0200a6a8(State0200a6a8 *p)
{
    if (p->object != 0) {
        delete p->object;
        p->object = 0;
    }
    p->next_time = 0;
    p->next_on = 0;
    p->on = p->next_on;
    p->time = p->next_time;
    p->mode = 0;
    p->next_level = 0;
    p->level = p->next_level;
}
