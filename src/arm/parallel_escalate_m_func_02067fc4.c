unsigned int *func_02067fac(unsigned int *, unsigned int *);

unsigned int *func_02067fc4(unsigned int *param_1, unsigned int param_2)
{
    unsigned int *node;
    unsigned int *found;

    for (node = func_02067fac(param_1, 0); node != 0; node = func_02067fac(param_1, node)) {
        if (node[6] <= param_2 && param_2 < node[7]) {
            found = func_02067fc4(node + 3, param_2);
            if (found == 0) {
                found = node;
            }
            return found;
        }
    }
    return 0;
}
