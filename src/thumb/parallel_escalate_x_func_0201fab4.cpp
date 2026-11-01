class Ent {
public:
  virtual void s0();
  virtual void s1();
  unsigned char pad4;
  unsigned char key;
  unsigned char b0 : 1;
  unsigned char b1 : 1;
  unsigned char b2 : 1;
  unsigned char b3 : 1;
  unsigned char b4 : 1;
  unsigned char b5 : 1;
  unsigned char b6 : 1;
  unsigned char b7 : 1;
  unsigned char tail;
  Ent *next;
};

extern "C" {
void *func_0201f698(int size);
void *func_0201f598(void *p);
}

struct Table {
  int pad0;
  int pad4;
  unsigned int count;
  int heads;
};

extern "C" void func_0201fab4(Table *param_1, unsigned int param_2)
{
  Ent **slot;
  Ent *e;
  Ent *head;
  Ent *cur;
  Ent *n;
  unsigned int i;
  int off;
  int base;

  for (i = 0; i < param_1->count; i++) {
    off = i * 4;
    for (e = *(Ent **)(param_1->heads + off); e != 0; e = e->next) {
      e->b7 = 0;
      e->b1 = 0;
    }
    base = param_1->heads;
    slot = (Ent **)(base + off);
    head = *slot;
    cur = head;
    if (head != 0) {
      while (cur->b6) {
        *slot = cur->next;
        cur->next = 0;
        if (cur != 0) {
          cur->s1();
        }
        base = param_1->heads;
        slot = (Ent **)(off + base);
        head = *slot;
        cur = head;
      }
      if (cur == 0 || cur->key > param_2) {
        for (; cur != 0 && cur->key > param_2; cur = cur->next) {
        }
        if (cur != 0) {
          n = (Ent *)func_0201f698(0xc);
          if (n != 0) {
            n = (Ent *)func_0201f598(n);
          }
          if (n != 0) {
            n->b6 = 1;
            n->key = cur->key;
            n->pad4 = cur->pad4;
            n->b2 = cur->b2;
            n->b1 = cur->b1;
            n->b3 = cur->b3;
            n->b4 = cur->b4;
            n->tail = cur->tail;
            n->next = *(Ent **)(param_1->heads + off);
            *(Ent **)(param_1->heads + off) = n;
          }
        }
        else if (head != 0) {
          head->b7 = 1;
        }
      }
    }
  }
}
