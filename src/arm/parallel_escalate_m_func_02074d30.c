typedef struct Cell02074d30 {
    struct Cell02074d30 *prev;
    struct Cell02074d30 *next;
    int size;
} Cell02074d30;

Cell02074d30 *func_02074d30(Cell02074d30 *list, Cell02074d30 *cell)
{
    Cell02074d30 *prev;
    Cell02074d30 *next;

    for (next = list, prev = 0; next; prev = next, next = next->next) {
        if (cell <= next) {
            break;
        }
    }
    cell->prev = prev;
    cell->next = next;
    if (next) {
        next->prev = cell;
        if ((char *)cell + cell->size == (char *)next) {
            cell->size += next->size;
            next = next->next;
            cell->next = next;
            if (next) {
                next->prev = cell;
            }
        }
    }
    if (prev) {
        prev->next = cell;
        if ((char *)prev + prev->size == (char *)cell) {
            prev->size += cell->size;
            prev->next = next;
            if (next) {
                next->prev = prev;
            }
        }
        return list;
    } else {
        return cell;
    }
}
