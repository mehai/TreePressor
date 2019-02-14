#ifndef AUXILIARY
#define AUXILIARY
#include"auxiliary.h"
#endif

QuadtreeNode *compRead(char *inputf, uint32_t *nrNodes, uint32_t *nrColors)
{
	QuadtreeNode *v = NULL;
	FILE *f = fopen(inputf, "rb");
	fread(nrColors, sizeof(uint32_t), 1, f);
	fread(nrNodes, sizeof(uint32_t), 1, f);
	v = (QuadtreeNode *)malloc(*nrNodes * sizeof(QuadtreeNode));
	fread(v, sizeof(QuadtreeNode), *nrNodes, f);
	fclose(f);
	return v;
}

void rebuildQTree(qtNode *node, QuadtreeNode *v, int k)
{
	node->area = v[k].area;
	node->red = v[k].red;
	node->blue = v[k].blue;
	node->green = v[k].green;
	/*daca nu mai are fii*/
	if(v[k].top_left == -1)
	{
		node->v[0] = NULL;
		node->v[1] = NULL;
		node->v[2] = NULL;
		node->v[3] = NULL;
	}
	else
	{
		/*aloc memorie pentru fii*/
		node->v[0] = (qtNode *)malloc(sizeof(qtNode));
		node->v[1] = (qtNode *)malloc(sizeof(qtNode));
		node->v[2] = (qtNode *)malloc(sizeof(qtNode));
		node->v[3] = (qtNode *)malloc(sizeof(qtNode));
		/*inserez datele in fii*/
		rebuildQTree(node->v[0], v, v[k].top_left);
		rebuildQTree(node->v[1], v, v[k].top_right);
		rebuildQTree(node->v[2], v, v[k].bottom_right);
		rebuildQTree(node->v[3], v, v[k].bottom_left);
	}
}

void decompress(char *inputf, char *outputf)
{
	int maxRGB = 255, i;
	uch **img;
	QuadtreeNode *v;
	qtNode *root = NULL;
	uint32_t nrNodes = 0, nrColors = 0;
	/*citesc datele din fisier*/
	v = compRead(inputf, &nrNodes, &nrColors);
	/*construiesc arborele*/
	root = (qtNode *)malloc(sizeof(qtNode));
	rebuildQTree(root, v, 0);
	/*stabilesc dimensiunea si coordonatele initiale*/
	Dimensions co;
	co.size = sqrt(v[0].area);
	co.x = 0;
	co.y = 0;
	/*construiesc matricea de pixeli*/
	img = (uch **)malloc(co.size * sizeof(uch*));
	for(i = 0; i < co.size; i++)
		img[i] = (uch*)malloc(3*co.size * sizeof(uch));
	buildImage(img, root, co);
	/*creez fisiserul ppm*/
	FILE *ppmfile = fopen(outputf, "wb");
	fprintf(ppmfile, "P6\n");
	fprintf(ppmfile, "%d %d\n", co.size, co.size);
	fprintf(ppmfile, "%d\n", 255);
	for(i = 0; i < co.size; i++)
		fwrite(img[i], sizeof(uch), co.size*3, ppmfile);
	fclose(ppmfile);
	/*eliberez memoria alocata dinamic*/
	free(v);
	freeMatrix(img, co.size);

}