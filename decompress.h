QuadtreeNode *compRead(char *inputf, uint32_t *nrNodes, uint32_t *nrColors);
void rebuildQTree(qtNode *node, QuadtreeNode *v, int k);
void decompress(char *inputf, char *outputf);
