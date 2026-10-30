typedef struct {
    unsigned char data[20];
} Entry0201d8ec;

typedef struct {
    unsigned int count;
    unsigned int head;
    unsigned char pad[0x1d60];
    Entry0201d8ec entries[30];
} Ring0201d8ec;

Entry0201d8ec *func_0201d8ec(Ring0201d8ec *ring, unsigned int index)
{
    if (index < 0x1e) {
        if (ring->count < 0x1e) {
            if (ring->head >= index) {
                return &ring->entries[ring->head - index];
            }
            return 0;
        }
        if (ring->head < index) {
            return &ring->entries[ring->count - (index - ring->head)];
        }
        return &ring->entries[ring->head - index];
    }
    return 0;
}
